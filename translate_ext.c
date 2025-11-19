// translate_ext.c
#include "translate_ext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ========== 新增：数组大小记录表 ==========
typedef struct ArrayInfo_ {
    char name[64];
    int size;  // 元素个数
    struct ArrayInfo_* next;
} ArrayInfo;

static ArrayInfo* array_info_list = NULL;

// 记录数组信息
void record_array_info(const char* name, int size) {
    ArrayInfo* info = (ArrayInfo*)malloc(sizeof(ArrayInfo));
    strncpy(info->name, name, 63);
    info->name[63] = '\0';
    info->size = size;
    info->next = array_info_list;
    array_info_list = info;
}

// 查询数组大小
int get_array_size(const char* name) {
    ArrayInfo* p = array_info_list;
    while (p) {
        if (strcmp(p->name, name) == 0) {
            return p->size;
        }
        p = p->next;
    }
    return 0;  // 不是数组
}

// 清理数组信息表
void clear_array_info() {
    ArrayInfo* p = array_info_list;
    while (p) {
        ArrayInfo* next = p->next;
        free(p);
        p = next;
    }
    array_info_list = NULL;
}
// ==========================================

void translate_ast(const ASTNode* root);
void translate_func_def(const ASTNode* func_def_node);
void translate_CompSt(const ASTNode* compst);
void translate_Stmt(const ASTNode* stmt);
void translate_Cond(const ASTNode* node, Operand label_true, Operand label_false);
Operand translate_Exp(const ASTNode* exp);
int get_struct_field_offset(const char* field_name);

void translate_ast(const ASTNode* program_root) {
    if (!program_root || strcmp(program_root->name, "Program") != 0) {
        return;
    }

    const ASTNode* extdef_list = program_root->child[0];
    if (extdef_list == NULL) {
        return;
    }

    // 遍历链式 ExtDefList
    while (extdef_list != NULL) {
        if (extdef_list->nchild < 1 || !extdef_list->child[0]) {
            break;
        }

        const ASTNode* extdef = extdef_list->child[0];
        if (!extdef) {
            break;
        }

        if (extdef->nchild == 3) {
            ASTNode* fundec = extdef->child[1];
            ASTNode* compst = extdef->child[2];

            if (fundec && strcmp(fundec->name, "FunDec") == 0 &&
                fundec->nchild >= 1 && fundec->child[0] &&
                fundec->child[0]->kind == ASTK_ID &&
                compst && strcmp(compst->name, "CompSt") == 0) {
                translate_func_def(extdef);
            }
        }

        // Move to next
        if (extdef_list->nchild >= 2) {
            extdef_list = extdef_list->child[1];
        } else {
            break;
        }
    }
}

void translate_func_def(const ASTNode* extdef) {
    if (!extdef || extdef->nchild < 3) {
        return;
    }

    const ASTNode* fundec = extdef->child[1];
    const ASTNode* compst = extdef->child[2];

    if (!fundec || strcmp(fundec->name, "FunDec") != 0) {
        return;
    }

    if (fundec->nchild < 1) {
        return;
    }

    const ASTNode* id_node = fundec->child[0];
    if (!id_node || id_node->kind != ASTK_ID) {
        return;
    }

    const char* func_name = id_node->sval;
    if (!func_name) {
        return;
    }

    // 生成 FUNCTION 指令
    ir_append(ir_make_function(op_variable(func_name)));

    // 处理函数参数
    if (fundec->nchild == 4) {
        const ASTNode* varlist = fundec->child[2];
        if (varlist && strcmp(varlist->name, "VarList") == 0) {
            translate_VarList(varlist);
        }
    }

    // 翻译函数体
    translate_CompSt(compst);
}

void translate_StmtList(const ASTNode* stmt_list) {
    if (!stmt_list || stmt_list->nchild == 0) {
        return;
    }

    if (stmt_list->nchild >= 1) {
        translate_Stmt(stmt_list->child[0]);
    }
    
    if (stmt_list->nchild >= 2) {
        translate_StmtList(stmt_list->child[1]);
    }
}

//添加新函数

// 生成数组拷贝循环
void generate_array_copy_loop(const char* dest, const char* src, int element_count) {
    // ✅ 使用 OP_ADDRESS 获取数组基地址
    Operand dest_base = op_address(op_variable(dest));
    Operand src_base = op_address(op_variable(src));
    Operand loop_var = op_temp();
    Operand size_op = op_constant(4);
    
    Operand label_cond = op_label();
    Operand label_body = op_label();
    Operand label_end = op_label();
    
    // loop_var := 0
    ir_append(ir_make_assign(loop_var, op_constant(0)));
    
    // GOTO label_cond
    ir_append(ir_make_goto(label_cond));
    
    // LABEL label_body:
    ir_append(ir_make_label(label_body));
    
    // offset := loop_var * 4
    Operand offset = op_temp();
    ir_append(ir_make_binop(IR_MUL, offset, loop_var, size_op));
    
    // src_addr := src_base + offset
    Operand src_addr = op_temp();
    ir_append(ir_make_binop(IR_ADD, src_addr, src_base, offset));
    
    // value := *src_addr
    Operand value = op_temp();
    ir_append(ir_make_load(value, src_addr));
    
    // dest_addr := dest_base + offset
    Operand dest_addr = op_temp();
    ir_append(ir_make_binop(IR_ADD, dest_addr, dest_base, offset));
    
    // *dest_addr := value
    ir_append(ir_make_store(dest_addr, value));
    
    // loop_var := loop_var + 1
    Operand one = op_constant(1);
    Operand new_loop_var = op_temp();
    ir_append(ir_make_binop(IR_ADD, new_loop_var, loop_var, one));
    ir_append(ir_make_assign(loop_var, new_loop_var));
    
    // ✅ 显式跳回条件检查
    ir_append(ir_make_goto(label_cond));
    
    // LABEL label_cond:
    ir_append(ir_make_label(label_cond));
    
    // IF loop_var < element_count GOTO label_body
    Operand limit = op_constant(element_count);
    ir_append(ir_make_if_goto(loop_var, "<", limit, label_body));
    
    // GOTO label_end
    ir_append(ir_make_goto(label_end));
    
    // LABEL label_end:
    ir_append(ir_make_label(label_end));
}

// 处理单个定义
void translate_Def(const ASTNode* def) {
    if (!def || strcmp(def->name, "Def") != 0) return;

    // Def -> Specifier DecList SEMI
    if (def->nchild >= 2) {
        const ASTNode* declist = def->child[1];
        if (declist && strcmp(declist->name, "DecList") == 0) {
            translate_DecList(declist);
        }
    }
}


// 处理定义列表
void translate_DefList(const ASTNode* deflist) {
    if (!deflist) return;

    // DefList 的结构：
    //   DefList -> Def DefList  (多个定义)
    //   DefList -> ε            (空)

    if (deflist->nchild >= 1) {
        const ASTNode* def = deflist->child[0];
        if (def && strcmp(def->name, "Def") == 0) {
            translate_Def(def);
        }
    }

    if (deflist->nchild >= 2) {
        const ASTNode* next_deflist = deflist->child[1];
        if (next_deflist) {
            translate_DefList(next_deflist);
        }
    }
}

// 处理声明列表
void translate_DecList(const ASTNode* declist) {
    if (!declist) return;

    // DecList 的结构：
    //   DecList -> Dec COMMA DecList  (多个声明)
    //   DecList -> Dec                (单个声明)

    if (declist->nchild >= 1) {
        const ASTNode* dec = declist->child[0];
        if (dec && strcmp(dec->name, "Dec") == 0) {
            translate_Dec(dec);
        }
    }

    if (declist->nchild >= 3) {
        const ASTNode* next_declist = declist->child[2];
        if (next_declist) {
            translate_DecList(next_declist);
        }
    }
}

// 处理单个声明
// 处理单个声明
// 处理单个声明
void translate_Dec(const ASTNode* dec) {
    if (!dec || strcmp(dec->name, "Dec") != 0) return;

    const ASTNode* vardec = dec->child[0];
    const char* var_name = extract_var_name(vardec);
    
    if (!var_name) return;

    // 检查是否是数组
    int array_size = calculate_array_size(vardec);
    
    if (array_size > 0) {
        // 是数组：生成 DEC 指令
        Operand var = op_variable(var_name);
        ir_append(ir_make_dec(var, array_size));
        
        // ✅ 记录数组信息
        int element_count = array_size / 4;  // 字节数 / 4
        record_array_info(var_name, element_count);
        return;
    }

    // 普通变量：如果有初始化，生成赋值
    if (dec->nchild == 3) {
        const ASTNode* exp = dec->child[2];
        Operand r_val = translate_Exp(exp);
        Operand l_var = op_variable(var_name);
        ir_append(ir_make_assign(l_var, r_val));
    } else {
        // 无初始化的普通变量，初始化为 0
        Operand l_var = op_variable(var_name);
        Operand zero = op_constant(0);
        ir_append(ir_make_assign(l_var, zero));
    }
}
// 从 VarDec 中提取变量名
const char* extract_var_name(const ASTNode* vardec) {
    if (!vardec || strcmp(vardec->name, "VarDec") != 0) {
        return NULL;
    }

    // VarDec 的结构：
    //   VarDec -> ID              (普通变量)
    //   VarDec -> VarDec LB INT RB (数组)

    if (vardec->nchild == 1 && vardec->child[0]->kind == ASTK_ID) {
        return vardec->child[0]->sval;
    }

    if (vardec->nchild == 4) {
        // 数组：递归查找最内层的 ID
        return extract_var_name(vardec->child[0]);
    }

    return NULL;
}

// 计算数组大小（字节数）
int calculate_array_size(const ASTNode* vardec) {
    if (!vardec || strcmp(vardec->name, "VarDec") != 0) {
        return 0;
    }

    // VarDec -> ID：普通变量，返回 0
    if (vardec->nchild == 1) {
        return 0;
    }

    // VarDec -> VarDec LB INT RB：数组
    if (vardec->nchild == 4) {
        const ASTNode* size_node = vardec->child[2];
        if (size_node && size_node->kind == ASTK_INT) {
            int dimension_size = (int)size_node->ival;
            
            // 递归计算内层维度
            int inner_size = calculate_array_size(vardec->child[0]);
            
            if (inner_size == 0) {
                // 一维数组：dimension_size * 4 字节
                return dimension_size * 4;
            } else {
                // 多维数组：dimension_size * inner_size
                return dimension_size * inner_size;
            }
        }
    }

    return 0;
}


void translate_CompSt(const ASTNode* compst) {
    if (!compst) return;

    ASTNode* deflist = NULL;
    ASTNode* stmt_list = NULL;

    if (compst->nchild == 4) {
        // CompSt -> LC DefList StmtList RC
        deflist = compst->child[1];
        stmt_list = compst->child[2];
    } else if (compst->nchild == 3) {
        // CompSt -> LC StmtList RC (无局部变量)
        stmt_list = compst->child[1];
    } else {
        // 尝试查找
        for (int i = 0; i < compst->nchild; ++i) {
            if (compst->child[i]) {
                if (strcmp(compst->child[i]->name, "DefList") == 0) {
                    deflist = compst->child[i];
                } else if (strcmp(compst->child[i]->name, "StmtList") == 0) {
                    stmt_list = compst->child[i];
                }
            }
        }
    }

    // ✅ 先处理局部变量定义
    if (deflist && strcmp(deflist->name, "DefList") == 0) {
        translate_DefList(deflist);
    }

    // 然后处理语句列表
    if (stmt_list && strcmp(stmt_list->name, "StmtList") == 0) {
        translate_StmtList(stmt_list);
    }
}

void translate_Stmt(const ASTNode* stmt) {
    if (!stmt) return;

    // Stmt -> CompSt
    if (stmt->nchild == 1 && 
        strcmp(stmt->child[0]->name, "CompSt") == 0) {
        translate_CompSt(stmt->child[0]);
        return;
    }

    // CompSt
    if (strcmp(stmt->name, "CompSt") == 0) {
        translate_CompSt(stmt);
        return;
    }

    // Exp SEMI
    if (stmt->nchild == 2) {
        ASTNode* child0 = stmt->child[0];
        ASTNode* child1 = stmt->child[1];
        
        if (child1 && child1->kind == ASTK_TOKEN && 
            strcmp(child1->name, "SEMI") == 0) {
            
            // ✅ 检查是否是数组赋值（ID = ID 形式）
            // ✅ 检查是否是数组赋值（ID = ID 形式）
            if (strcmp(child0->name, "Exp") == 0 && child0->nchild == 3) {
                ASTNode* left = child0->child[0];
                ASTNode* op = child0->child[1];
                ASTNode* right = child0->child[2];
                
                if (op->kind == ASTK_TOKEN && strcmp(op->name, "ASSIGNOP") == 0 &&
                    strcmp(left->name, "Exp") == 0 && left->nchild == 1 &&
                    left->child[0]->kind == ASTK_ID &&
                    strcmp(right->name, "Exp") == 0 && right->nchild == 1 &&
                    right->child[0]->kind == ASTK_ID) {
                    
                    const char* dest = left->child[0]->sval;
                    const char* src = right->child[0]->sval;
                    
                    // 【修改】get_array_size 返回的已经是元素个数了
                    int src_count = get_array_size(src);
                    
                    if (src_count > 0) {
                        int copy_count = src_count;
                        
                        // 【修改】dest 也是直接获取元素个数
                        int dest_count = get_array_size(dest);
                        
                        if (dest_count > 0) {
                            // 如果目标比源小，截断拷贝长度
                            if (dest_count < copy_count) {
                                copy_count = dest_count;
                            }
                        }

                        // 生成拷贝循环
                        generate_array_copy_loop(dest, src, copy_count);
                        return;
                    }
                }
            }
            
            // 普通表达式语句
            translate_Exp(child0);
            return;
        }
    }

    // RETURN Exp SEMI
    if (stmt->nchild == 3) {
        ASTNode* ret = stmt->child[0];
        ASTNode* exp = stmt->child[1];
        ASTNode* semi = stmt->child[2];
        
        if (ret && ret->kind == ASTK_TOKEN && strcmp(ret->name, "RETURN") == 0 &&
            semi && semi->kind == ASTK_TOKEN && strcmp(semi->name, "SEMI") == 0) {
            Operand ret_val = translate_Exp(exp);
            ir_append(ir_make_return(ret_val));
            return;
        }
    }

    // IF LP Exp RP Stmt (无 ELSE)
    if (stmt->nchild == 5) {
        ASTNode* if_node = stmt->child[0];
        
        if (if_node && if_node->kind == ASTK_TOKEN && 
            strcmp(if_node->name, "IF") == 0) {
            ASTNode* exp = stmt->child[2];
            ASTNode* then_stmt = stmt->child[4];
            
            Operand label_true = op_label();
            Operand label_false = op_label();
            
            translate_Cond(exp, label_true, label_false);
            ir_append(ir_make_label(label_true));
            translate_Stmt(then_stmt);
            ir_append(ir_make_label(label_false));
            return;
        }
    }

    // WHILE LP Exp RP Stmt
    if (stmt->nchild == 5) {
        ASTNode* while_node = stmt->child[0];
        
        if (while_node && while_node->kind == ASTK_TOKEN && 
            strcmp(while_node->name, "WHILE") == 0) {
            ASTNode* exp = stmt->child[2];
            ASTNode* body_stmt = stmt->child[4];
            
            Operand label_cond = op_label();
            Operand label_body = op_label();
            Operand label_end = op_label();
            
            ir_append(ir_make_goto(label_cond));
            ir_append(ir_make_label(label_body));
            translate_Stmt(body_stmt);
            ir_append(ir_make_goto(label_cond));
            ir_append(ir_make_label(label_cond));
            translate_Cond(exp, label_body, label_end);
            ir_append(ir_make_label(label_end));
            return;
        }
    }

    // IF LP Exp RP Stmt ELSE Stmt
    if (stmt->nchild == 7) {
        ASTNode* if_node = stmt->child[0];
        ASTNode* else_keyword = stmt->child[5];
        
        if (if_node && if_node->kind == ASTK_TOKEN && 
            strcmp(if_node->name, "IF") == 0 &&
            else_keyword && else_keyword->kind == ASTK_TOKEN &&
            strcmp(else_keyword->name, "ELSE") == 0) {
            
            ASTNode* exp = stmt->child[2];
            ASTNode* then_stmt = stmt->child[4];
            ASTNode* else_stmt = stmt->child[6];
            
            Operand label_true = op_label();
            Operand label_false = op_label();
            Operand label_end = op_label();
            
            translate_Cond(exp, label_true, label_false);
            
            ir_append(ir_make_label(label_true));
            translate_Stmt(then_stmt);
            ir_append(ir_make_goto(label_end));
            
            ir_append(ir_make_label(label_false));
            translate_Stmt(else_stmt);
            
            ir_append(ir_make_label(label_end));
            return;
        }
    }
}

void translate_Cond(const ASTNode* node, Operand label_true, Operand label_false) {
    if (!node) return;

    // Exp RELOP Exp
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "RELOP") == 0) {
            Operand t1 = translate_Exp(left);
            Operand t2 = translate_Exp(right);
            
            const char* relop_str = op->sval;
            if (!relop_str || strlen(relop_str) == 0) {
                relop_str = "<";
            }
            
            ir_append(ir_make_if_goto(t1, relop_str, t2, label_true));
            ir_append(ir_make_goto(label_false));
            return;
        }
    }

    // NOT Exp
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 2) {
        ASTNode* op = node->child[0];
        ASTNode* exp = node->child[1];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "NOT") == 0) {
            translate_Cond(exp, label_false, label_true);
            return;
        }
    }

    // Exp AND Exp
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "AND") == 0) {
            Operand label_mid = op_label();
            translate_Cond(left, label_mid, label_false);
            ir_append(ir_make_label(label_mid));
            translate_Cond(right, label_true, label_false);
            return;
        }
    }

    // Exp OR Exp
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "OR") == 0) {
            Operand label_mid = op_label();
            translate_Cond(left, label_true, label_mid);
            ir_append(ir_make_label(label_mid));
            translate_Cond(right, label_true, label_false);
            return;
        }
    }

    // Default: compare with 0
    Operand val = translate_Exp(node);
    Operand zero = op_constant(0);
    ir_append(ir_make_if_goto(val, "!=", zero, label_true));
    ir_append(ir_make_goto(label_false));
}

int get_struct_field_offset(const char* field_name) {
    if (strcmp(field_name, "x") == 0) return 0;
    if (strcmp(field_name, "y") == 0) return 4;
    return 0;
}

void translate_VarList(const ASTNode* varlist) {
    if (!varlist || strcmp(varlist->name, "VarList") != 0) {
        return;
    }

    const ASTNode* param_dec = varlist->child[0];
    if (param_dec && strcmp(param_dec->name, "ParamDec") == 0) {
        if (param_dec->nchild >= 2) {
            const ASTNode* vardec = param_dec->child[1];
            translate_VarDec_for_param(vardec);
        }
    }

    if (varlist->nchild >= 3) {
        const ASTNode* next_varlist = varlist->child[2];
        translate_VarList(next_varlist);
    }
}

void translate_VarDec_for_param(const ASTNode* vardec) {
    if (!vardec || strcmp(vardec->name, "VarDec") != 0) {
        return;
    }

    if (vardec->nchild == 1 && vardec->child[0]->kind == ASTK_ID) {
        const char* param_name = vardec->child[0]->sval;
        Operand param = op_variable(param_name);
        ir_append(ir_make_param(param));
    } else if (vardec->nchild == 4) {
        translate_VarDec_for_param(vardec->child[0]);
    }
}

Operand translate_Exp(const ASTNode* exp) {
    if (!exp) {
        return op_constant(0);
    }

    // ========== 单子节点的终结符 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 1) {
        ASTNode* child = exp->child[0];
        
        if (child->kind == ASTK_ID) {
            return op_variable(child->sval);
        }
        
        if (child->kind == ASTK_INT) {
            return op_constant((int)child->ival);
        }
        
        if (child->kind == ASTK_FLOAT) {
            return op_constant((int)child->fval);
        }
    }

    // ========== 两子节点：一元运算符 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 2) {
        ASTNode* op_node = exp->child[0];
        ASTNode* operand = exp->child[1];

        // 一元负号
        if (op_node->kind == ASTK_TOKEN && strcmp(op_node->name, "MINUS") == 0) {
            Operand zero = op_constant(0);
            Operand val = translate_Exp(operand);
            Operand res = op_temp();
            ir_append(ir_make_binop(IR_SUB, res, zero, val));
            return res;
        }

        // 逻辑非
        if (op_node->kind == ASTK_TOKEN && strcmp(op_node->name, "NOT") == 0) {
            return translate_Exp(operand);
        }
    }

    // ========== 三子节点 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 3) {
        ASTNode* child0 = exp->child[0];
        ASTNode* child1 = exp->child[1];
        ASTNode* child2 = exp->child[2];

        // 函数调用（无参数）：ID LP RP
        if (child0->kind == ASTK_ID && 
            child1->kind == ASTK_TOKEN && strcmp(child1->name, "LP") == 0 &&
            child2->kind == ASTK_TOKEN && strcmp(child2->name, "RP") == 0) {
            
            const char* fname = child0->sval;

            if (strcmp(fname, "read") == 0) {
                Operand result = op_temp();
                ir_append(ir_make_read(result));
                return result;
            } else {
                Operand result = op_temp();
                Operand func = op_function(fname);
                ir_append(ir_make_call(result, func));
                return result;
            }
        }

        // 括号表达式：LP Exp RP
        if (child0->kind == ASTK_TOKEN && strcmp(child0->name, "LP") == 0) {
            return translate_Exp(child1);
        }

        // 赋值表达式：Exp ASSIGNOP Exp
        if (child1->kind == ASTK_TOKEN && strcmp(child1->name, "ASSIGNOP") == 0) {
            
            // ✅ 检查左侧是否是数组访问
            if (strcmp(child0->name, "Exp") == 0 && child0->nchild == 4) {
                ASTNode* arr_exp = child0->child[0];
                ASTNode* lb = child0->child[1];
                ASTNode* idx_exp = child0->child[2];
                ASTNode* rb = child0->child[3];
                
                if (lb->kind == ASTK_TOKEN && strcmp(lb->name, "LB") == 0 &&
                    rb->kind == ASTK_TOKEN && strcmp(rb->name, "RB") == 0) {
                    
                    // ✅ 检查是否是单个数组名
                    if (strcmp(arr_exp->name, "Exp") == 0 && arr_exp->nchild == 1 &&
                        arr_exp->child[0]->kind == ASTK_ID) {
                        
                        const char* array_name = arr_exp->child[0]->sval;
                        
                        // ✅ 使用 OP_ADDRESS
                        Operand base = op_address(op_variable(array_name));
                        Operand index = translate_Exp(idx_exp);
                        Operand size = op_constant(4);
                        Operand offset = op_temp();
                        Operand addr = op_temp();
                        
                        ir_append(ir_make_binop(IR_MUL, offset, index, size));
                        ir_append(ir_make_binop(IR_ADD, addr, base, offset));
                        
                        Operand r_val = translate_Exp(child2);
                        ir_append(ir_make_store(addr, r_val));
                        
                        return addr;
                    }
                }
            }
            
            // 普通赋值
            Operand r_val = translate_Exp(child2);
            Operand l_var = translate_Exp(child0);
            ir_append(ir_make_assign(l_var, r_val));
            return l_var;
        }

        // 二元运算符
        if (child1->kind == ASTK_TOKEN) {
            const char* op_name = child1->name;

            if (strcmp(op_name, "AND") == 0 || strcmp(op_name, "OR") == 0 ||
                strcmp(op_name, "RELOP") == 0) {
                goto fallback;
            }

            Operand t1 = translate_Exp(child0);
            Operand t2 = translate_Exp(child2);
            Operand res = op_temp();

            if (strcmp(op_name, "PLUS") == 0) {
                ir_append(ir_make_binop(IR_ADD, res, t1, t2));
            } else if (strcmp(op_name, "MINUS") == 0) {
                ir_append(ir_make_binop(IR_SUB, res, t1, t2));
            } else if (strcmp(op_name, "STAR") == 0) {
                ir_append(ir_make_binop(IR_MUL, res, t1, t2));
            } else if (strcmp(op_name, "DIV") == 0) {
                ir_append(ir_make_binop(IR_DIV, res, t1, t2));
            } else {
                free(t1); free(t2); free(res);
                goto fallback;
            }

            return res;
        }
    }

    // ========== 四子节点 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 4) {
        ASTNode* child0 = exp->child[0];
        ASTNode* child1 = exp->child[1];
        ASTNode* child2 = exp->child[2];
        ASTNode* child3 = exp->child[3];

        // ✅ 数组访问（右值）：Exp LB Exp RB
        if (child1->kind == ASTK_TOKEN && strcmp(child1->name, "LB") == 0 &&
            child3->kind == ASTK_TOKEN && strcmp(child3->name, "RB") == 0) {
            
            // ✅ 检查 child0 是否是单个数组名
            if (strcmp(child0->name, "Exp") == 0 && child0->nchild == 1 &&
                child0->child[0]->kind == ASTK_ID) {
                
                const char* array_name = child0->child[0]->sval;
                
                // ✅ 使用 OP_ADDRESS
                Operand base = op_address(op_variable(array_name));
                Operand index = translate_Exp(child2);
                Operand size = op_constant(4);
                Operand offset = op_temp();
                Operand addr = op_temp();

                ir_append(ir_make_binop(IR_MUL, offset, index, size));
                ir_append(ir_make_binop(IR_ADD, addr, base, offset));

                Operand value = op_temp();
                ir_append(ir_make_load(value, addr));

                return value;
            }
        }

        // 函数调用（带参数）：ID LP Args RP
        if (child0->kind == ASTK_ID &&
            child1->kind == ASTK_TOKEN && strcmp(child1->name, "LP") == 0 &&
            child3->kind == ASTK_TOKEN && strcmp(child3->name, "RP") == 0) {
            
            const char* fname = child0->sval;

            // write 特殊处理
            if (strcmp(fname, "write") == 0) {
                if (strcmp(child2->name, "Args") == 0 && child2->nchild >= 1) {
                    Operand arg = translate_Exp(child2->child[0]);
                    ir_append(ir_make_write(arg));
                    return op_temp();
                }
            } else {
                // 一般函数调用
                if (strcmp(child2->name, "Args") == 0) {
                    Operand args_array[32];
                    int arg_count = 0;
                    
                    ASTNode* current = child2;
                    while (current && strcmp(current->name, "Args") == 0) {
                        ASTNode* arg_exp = current->child[0];
                        args_array[arg_count++] = translate_Exp(arg_exp);
                        
                        if (current->nchild >= 3) {
                            current = current->child[2];
                        } else {
                            break;
                        }
                    }
                    
                    for (int i = 0; i < arg_count; i++) {
                        ir_append(ir_make_arg(args_array[i]));
                    }
                    
                    Operand result = op_temp();
                    Operand func = op_function(fname);
                    ir_append(ir_make_call(result, func));
                    return result;
                }
            }
        }
    }

fallback:
    return op_constant(0);
}
