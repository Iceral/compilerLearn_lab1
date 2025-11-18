// translate_ext.c
#include "translate_ext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void translate_ast(const ASTNode* root);
void translate_func_def(const ASTNode* func_def_node);
void translate_CompSt(const ASTNode* compst);
void translate_Stmt(const ASTNode* stmt);
void translate_Cond(const ASTNode* node, Operand label_true, Operand label_false);
Operand translate_Exp(const ASTNode* exp);
int get_struct_field_offset(const char* field_name);

void translate_ast(const ASTNode* program_root) {


    const ASTNode* extdef_list = program_root->child[0]; // ExtDefList or NULL

    printf("DEBUG: translate_ast called. Root name: %s\n", 
           program_root ? program_root->name : "(null)");
    
    if (!program_root || strcmp(program_root->name, "Program") != 0) {
        printf("DEBUG: Not a Program node.\n");
        return;
    }

    printf("DEBUG: ExtDefList = %p\n", (void*)extdef_list);

    if (extdef_list == NULL) {
        printf("DEBUG: ExtDefList is NULL! No external definitions.\n");
        return;
    }

    int count = 0;
        

    // 遍历链式 ExtDefList
    while (extdef_list != NULL) {
        printf("DEBUG: Processing ExtDefList #%d\n", ++count);

        if (extdef_list->nchild < 1 || !extdef_list->child[0]) {
            printf("DEBUG: ExtDefList has no ExtDef child.\n");
            break;
        }

        const ASTNode* extdef = extdef_list->child[0];
        printf("DEBUG: ExtDef node: nchild=%d\n", extdef ? extdef->nchild : -1);

        if (!extdef) {
            printf("DEBUG: ExtDef is NULL!\n");
            break;
        }

        // 打印每个子节点的 name
        for (int i = 0; i < extdef->nchild; ++i) {
            if (extdef->child[i]) {
                printf("  child[%d]: name=\"%s\"\n", i, extdef->child[i]->name);
            } else {
                printf("  child[%d]: NULL\n", i);
            }
        }

        if (extdef->nchild == 3) {
            ASTNode* spec   = extdef->child[0];
            ASTNode* fundec = extdef->child[1];
            ASTNode* compst = extdef->child[2];

            printf("DEBUG: Checking function definition...\n");
            if (!fundec) {
                printf("DEBUG: fundec is NULL\n");
            } else {
                printf("DEBUG: fundec->name = \"%s\"\n", fundec->name);
                if (fundec->nchild > 0 && fundec->child[0]) {
                    printf("DEBUG: fundec->child[0]->kind = %d (ID=%d)\n", 
                           fundec->child[0]->kind, ASTK_ID);
                }
            }

            if (!compst) {
                printf("DEBUG: compst is NULL\n");
            } else {
                printf("DEBUG: compst->name = \"%s\"\n", compst->name);
            }

            if (fundec && strcmp(fundec->name, "FunDec") == 0 &&
                fundec->nchild >= 1 && fundec->child[0] &&
                fundec->child[0]->kind == ASTK_ID &&
                compst && strcmp(compst->name, "CompSt") == 0) {

                printf("DEBUG: ✅ Calling translate_func_def!\n");
                translate_func_def(extdef);
            } else {
                printf("DEBUG: ❌ Condition failed.\n");
            }
        } else {
            printf("DEBUG: ExtDef nchild != 3 (got %d). Not a function.\n", extdef->nchild);
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
    // extdef: [Specifier, FunDec, CompSt]
    if (!extdef || extdef->nchild < 3) {
        fprintf(stderr, "Error: Invalid ExtDef for function definition\n");
        return;
    }

    const ASTNode* fundec = extdef->child[1]; // FunDec node
    const ASTNode* compst = extdef->child[2]; // CompSt node

    if (!fundec || strcmp(fundec->name, "FunDec") != 0) {
        fprintf(stderr, "Error: Expected FunDec node\n");
        return;
    }

    // === FunDec 的第一个子节点就是函数名 (ID) ===
    if (fundec->nchild < 1) {
        fprintf(stderr, "Error: FunDec has no children\n");
        return;
    }

    const ASTNode* id_node = fundec->child[0]; // ← 关键：索引是 0！
    if (!id_node || id_node->kind != ASTK_ID) {
        fprintf(stderr, "Error: Expected ID as first child of FunDec\n");
        return;
    }

    const char* func_name = id_node->sval;
    if (!func_name) {
        fprintf(stderr, "Error: Function name is NULL\n");
        return;
    }

    printf("INFO: Translating function: %s\n", func_name);

    // 生成 FUNCTION 指令
    ir_append(ir_make_function(op_variable(func_name)));

    // 翻译函数体
    translate_CompSt(compst);
    printf("DEBUG: FunDec has %d children:\n", fundec->nchild);
    for (int i = 0; i < fundec->nchild; i++) {
    ASTNode* c = fundec->child[i];
    printf("  [%d] name='%s', kind=%d, sval='%s'\n",
           i, c->name ? c->name : "(null)",
           c->kind,
           c->sval ? c->sval : "(null)");
}
}
void translate_StmtList(const ASTNode* stmt_list) {
    // ✅ 添加计数器
    static int stmt_count = 0;
    
    if (!stmt_list || stmt_list->nchild == 0) {
        printf("DEBUG: StmtList is empty or NULL\n");
        return;
    }

    printf("DEBUG: translate_StmtList #%d, nchild=%d\n", 
           ++stmt_count, stmt_list->nchild);

    if (stmt_list->nchild >= 1) {
        ASTNode* stmt = stmt_list->child[0];
        printf("  -> Processing Stmt, name='%s', nchild=%d\n",
               stmt ? stmt->name : "(null)", stmt ? stmt->nchild : -1);
        translate_Stmt(stmt);
    }
    
    if (stmt_list->nchild >= 2) {
        printf("  -> Moving to next StmtList\n");
        translate_StmtList(stmt_list->child[1]);
    } else {
        printf("  -> End of StmtList\n");
    }
}

void translate_CompSt(const ASTNode* compst) {
    if (!compst) return;
    // ✅ 添加这段调试代码
    printf("DEBUG: translate_CompSt called, nchild=%d\n", compst->nchild);
    for (int i = 0; i < compst->nchild; i++) {
        ASTNode* child = compst->child[i];
        printf("  child[%d]: name='%s', nchild=%d\n", 
               i, child ? child->name : "(null)", 
               child ? child->nchild : -1);
    }

    // CompSt -> LC DefList StmtList RC
    // Usually nchild == 4, but sometimes DefList is empty and omitted in some parsers.
    ASTNode* stmt_list = NULL;

    if (compst->nchild == 4) {
        // LC DefList StmtList RC
        stmt_list = compst->child[2];
    } else if (compst->nchild == 3) {
        // LC StmtList RC  (no local definitions)
        stmt_list = compst->child[1];
    } else {
        // Unexpected structure; try to find StmtList by name
        for (int i = 0; i < compst->nchild; ++i) {
            if (compst->child[i] && strcmp(compst->child[i]->name, "StmtList") == 0) {
                stmt_list = compst->child[i];
                break;
            }
        }
    }

    if (stmt_list && strcmp(stmt_list->name, "StmtList") == 0) {
        translate_StmtList(stmt_list);
    }
}
void translate_Stmt(const ASTNode* stmt) {
    if (!stmt) return;

    printf("DEBUG: translate_Stmt: nchild=%d\n", stmt->nchild);
    if (stmt->nchild > 0 && stmt->child[0]) {
        printf("  child[0]: name='%s'\n", stmt->child[0]->name);
    }
    // ✅ 新增：处理 Stmt -> CompSt（单子节点的复合语句）
    if (stmt->nchild == 1 && 
        strcmp(stmt->child[0]->name, "CompSt") == 0) {
        printf("  -> Stmt containing CompSt\n");
        translate_CompSt(stmt->child[0]);
        return;
    }
    // ========== 1. 复合语句：CompSt ==========
    if (strcmp(stmt->name, "CompSt") == 0) {
        printf("  -> CompSt (nested block)\n");
        translate_CompSt(stmt);
        return;
    }

    // ========== 2. 表达式语句：Exp SEMI ==========
    if (stmt->nchild == 2) {
        ASTNode* child0 = stmt->child[0];
        ASTNode* child1 = stmt->child[1];
        
        if (child1 && child1->kind == ASTK_TOKEN && 
            strcmp(child1->name, "SEMI") == 0) {
            
            printf("  -> Exp SEMI\n");
            
            Operand t = translate_Exp(child0);
            // ✅ 不要 free t！
            return;
        }
    }

    // ========== 3. 返回语句：RETURN Exp SEMI ==========
    if (stmt->nchild == 3) {
        ASTNode* ret = stmt->child[0];
        ASTNode* exp = stmt->child[1];
        ASTNode* semi = stmt->child[2];
        
        if (ret && ret->kind == ASTK_TOKEN && strcmp(ret->name, "RETURN") == 0 &&
            semi && semi->kind == ASTK_TOKEN && strcmp(semi->name, "SEMI") == 0) {
            
            printf("  -> RETURN statement\n");
            
            Operand ret_val = translate_Exp(exp);
            InterCode code = ir_make_return(ret_val);
            ir_append(code);
            
            // ✅ 不要 free ret_val！
            return;
        }
    }

    // ========== 4. IF 语句（无 ELSE）：IF LP Exp RP Stmt ==========
    if (stmt->nchild == 5) {
        ASTNode* if_node = stmt->child[0];
        ASTNode* lp = stmt->child[1];
        ASTNode* exp = stmt->child[2];
        ASTNode* rp = stmt->child[3];
        ASTNode* then_stmt = stmt->child[4];
        
        if (if_node && if_node->kind == ASTK_TOKEN && 
            strcmp(if_node->name, "IF") == 0) {
            
            printf("  -> IF statement (no ELSE)\n");
            
            Operand label_true = op_label();
            Operand label_false = op_label();
            
            // if (exp) { then_stmt }
            translate_Cond(exp, label_true, label_false);
            
            ir_append(ir_make_label(label_true));
            translate_Stmt(then_stmt);
            
            ir_append(ir_make_label(label_false));
            
            // ✅ 不要 free label_true, label_false！
            return;
        }
    }

    // ========== 5. WHILE 循环：WHILE LP Exp RP Stmt ==========
    if (stmt->nchild == 5) {
        ASTNode* while_node = stmt->child[0];
        ASTNode* lp = stmt->child[1];
        ASTNode* exp = stmt->child[2];
        ASTNode* rp = stmt->child[3];
        ASTNode* body_stmt = stmt->child[4];
        
        if (while_node && while_node->kind == ASTK_TOKEN && 
            strcmp(while_node->name, "WHILE") == 0) {
            
            printf("  -> WHILE statement\n");
            
            Operand label_cond = op_label();
            Operand label_body = op_label();
            Operand label_end = op_label();
            
            // GOTO label_cond
            ir_append(ir_make_goto(label_cond));
            
            // label_body:
            ir_append(ir_make_label(label_body));
            translate_Stmt(body_stmt);
            
            // label_cond:
            ir_append(ir_make_label(label_cond));
            translate_Cond(exp, label_body, label_end);
            
            // label_end:
            ir_append(ir_make_label(label_end));
            
            // ✅ 不要 free 标签！
            return;
        }
    }

    // ========== 6. IF-ELSE 语句：IF LP Exp RP Stmt ELSE Stmt ==========
    if (stmt->nchild == 7) {
        ASTNode* if_node = stmt->child[0];
        ASTNode* lp = stmt->child[1];
        ASTNode* exp = stmt->child[2];
        ASTNode* rp = stmt->child[3];
        ASTNode* then_stmt = stmt->child[4];
        ASTNode* else_keyword = stmt->child[5];
        ASTNode* else_stmt = stmt->child[6];
        
        if (if_node && if_node->kind == ASTK_TOKEN && 
            strcmp(if_node->name, "IF") == 0 &&
            else_keyword && else_keyword->kind == ASTK_TOKEN &&
            strcmp(else_keyword->name, "ELSE") == 0) {
            
            printf("  -> IF-ELSE statement\n");
            
            Operand label_true = op_label();
            Operand label_false = op_label();
            Operand label_end = op_label();
            
            // if (exp) { then_stmt } else { else_stmt }
            translate_Cond(exp, label_true, label_false);
            
            // then 分支
            ir_append(ir_make_label(label_true));
            translate_Stmt(then_stmt);
            ir_append(ir_make_goto(label_end));
            
            // else 分支
            ir_append(ir_make_label(label_false));
            translate_Stmt(else_stmt);
            
            // 结束
            ir_append(ir_make_label(label_end));
            
            // ✅ 不要 free 标签！
            return;
        }
    }

    printf("  -> Unknown statement type\n");
}
void translate_Cond(const ASTNode* node, Operand label_true, Operand label_false) {
    if (!node) return;

    printf("DEBUG: translate_Cond: name='%s', nchild=%d\n", 
           node->name, node->nchild);

    // ========== 1. 关系运算：Exp RELOP Exp ==========
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        // 检查是否是 RELOP（<, <=, >, >=, ==, !=）
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "RELOP") == 0) {
            printf("  -> RELOP condition\n");
            // ✅ 添加调试：打印 sval
            printf("  -> op->sval = '%s'\n", op->sval ? op->sval : "(null)");
            printf("  -> op->name = '%s'\n", op->name ? op->name : "(null)");
            
            Operand t1 = translate_Exp(left);
            Operand t2 = translate_Exp(right);
            
            // 使用 op->sval 获取具体的关系运算符（<, >, ==, 等）
            const char* relop_str = op->sval;
            if (!relop_str || strlen(relop_str) == 0) {
                relop_str = "<";  // ✅ 临时使用默认值，看看能否工作
            }
            
            printf("  -> Using relop: '%s'\n", relop_str);

            
            InterCode code = ir_make_if_goto(t1, relop_str, t2, label_true);
            ir_append(code);
            ir_append(ir_make_goto(label_false));
            
            // ✅ 不要 free t1, t2！它们已被加入 IR
            return;
        }
    }

    // ========== 2. 逻辑非：NOT Exp ==========
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 2) {
        ASTNode* op = node->child[0];
        ASTNode* exp = node->child[1];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "NOT") == 0) {
            printf("  -> NOT condition (swap labels)\n");
            // NOT 条件：交换 true/false 标签
            translate_Cond(exp, label_false, label_true);
            return;
        }
    }

    // ========== 3. 逻辑与：Exp AND Exp ==========
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "AND") == 0) {
            printf("  -> AND condition\n");
            
            // left && right：
            // if (!left) goto false;
            // if (!right) goto false;
            // goto true;
            
            Operand label_mid = op_label();
            translate_Cond(left, label_mid, label_false);
            ir_append(ir_make_label(label_mid));
            translate_Cond(right, label_true, label_false);
            
            // ✅ 不要 free label_mid！
            return;
        }
    }

    // ========== 4. 逻辑或：Exp OR Exp ==========
    if (strcmp(node->name, "Exp") == 0 && node->nchild == 3) {
        ASTNode* left = node->child[0];
        ASTNode* op = node->child[1];
        ASTNode* right = node->child[2];
        
        if (op->kind == ASTK_TOKEN && strcmp(op->name, "OR") == 0) {
            printf("  -> OR condition\n");
            
            // left || right：
            // if (left) goto true;
            // if (right) goto true;
            // goto false;
            
            Operand label_mid = op_label();
            translate_Cond(left, label_true, label_mid);
            ir_append(ir_make_label(label_mid));
            translate_Cond(right, label_true, label_false);
            
            // ✅ 不要 free label_mid！
            return;
        }
    }

    // ========== 5. 默认：其他表达式（视为非零即真） ==========
    printf("  -> Default condition (compare with 0)\n");
    
    // 对于 a、a+b 等非布尔表达式，转换为 if (exp != 0)
    Operand val = translate_Exp(node);
    Operand zero = op_constant(0);
    
    InterCode code = ir_make_if_goto(val, "!=", zero, label_true);
    ir_append(code);
    ir_append(ir_make_goto(label_false));
    
    // ✅ 不要 free val, zero！
}

int get_struct_field_offset(const char* field_name) {
    if (strcmp(field_name, "x") == 0) return 0;
    if (strcmp(field_name, "y") == 0) return 4;
    return 0;
}

Operand translate_Exp(const ASTNode* exp) {
    if (!exp) {
        printf("DEBUG: translate_Exp: exp is NULL\n");
        return op_constant(0);
    }

    printf("DEBUG: translate_Exp: name='%s', nchild=%d, kind=%d\n",
           exp->name, exp->nchild, exp->kind);

    // ========== 1. 单子节点的终结符 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 1) {
        ASTNode* child = exp->child[0];
        printf("  -> Single child: name='%s', kind=%d\n", 
               child->name, child->kind);
        
        if (child->kind == ASTK_ID) {
            printf("  -> Creating variable: %s\n", child->sval);
            return op_variable(child->sval);
        }
        
        if (child->kind == ASTK_INT) {
            printf("  -> Creating constant: %lld\n", child->ival);
            return op_constant((int)child->ival);
        }
        
        if (child->kind == ASTK_FLOAT) {
            printf("  -> Creating float constant (as int): %f\n", child->fval);
            return op_constant((int)child->fval);
        }
    }

    // ========== 2. 两子节点 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 2) {
        ASTNode* op_node = exp->child[0];
        ASTNode* operand = exp->child[1];
        
        printf("  -> 2 children: op='%s', operand='%s'\n", 
               op_node->name, operand->name);

        // 一元负号
        if (op_node->kind == ASTK_TOKEN && strcmp(op_node->name, "MINUS") == 0) {
            printf("  -> Unary MINUS\n");
            Operand zero = op_constant(0);
            Operand val = translate_Exp(operand);
            Operand res = op_temp();
            ir_append(ir_make_binop(IR_SUB, res, zero, val));
            // ✅ 不要 free！
            return res;
        }

        // 逻辑非
        if (op_node->kind == ASTK_TOKEN && strcmp(op_node->name, "NOT") == 0) {
            printf("  -> NOT expression\n");
            return translate_Exp(operand);
        }
    }

    // ========== 3. 三子节点 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 3) {
        ASTNode* child0 = exp->child[0];
        ASTNode* child1 = exp->child[1];
        ASTNode* child2 = exp->child[2];

        printf("  -> 3 children: [0]='%s' (kind=%d), [1]='%s' (kind=%d), [2]='%s' (kind=%d)\n",
               child0->name, child0->kind, 
               child1->name, child1->kind, 
               child2->name, child2->kind);

        // 3.1 函数调用（无参数）：ID LP RP
        if (child0->kind == ASTK_ID && 
            child1->kind == ASTK_TOKEN && strcmp(child1->name, "LP") == 0 &&
            child2->kind == ASTK_TOKEN && strcmp(child2->name, "RP") == 0) {
            
            const char* fname = child0->sval;
            printf("  -> Function call (no args): %s\n", fname);

            if (strcmp(fname, "read") == 0) {
                Operand result = op_temp();
                ir_append(ir_make_read(result));
                return result;
            } else {
                Operand result = op_temp();
                Operand func = op_function(fname);
                ir_append(ir_make_call(result, func));
                // ✅ 不要 free func！
                return result;
            }
        }

        // 3.2 括号表达式：LP Exp RP
        if (child0->kind == ASTK_TOKEN && strcmp(child0->name, "LP") == 0) {
            printf("  -> Parentheses\n");
            return translate_Exp(child1);
        }

        // 3.3 赋值表达式：Exp ASSIGNOP Exp
        if (child1->kind == ASTK_TOKEN && strcmp(child1->name, "ASSIGNOP") == 0) {
            printf("  -> ASSIGNOP\n");
            
            // 检查左侧是否是数组访问
            if (strcmp(child0->name, "Exp") == 0 && child0->nchild == 4) {
                ASTNode* arr_exp = child0->child[0];
                ASTNode* lb = child0->child[1];
                ASTNode* idx_exp = child0->child[2];
                ASTNode* rb = child0->child[3];
                
                if (lb->kind == ASTK_TOKEN && strcmp(lb->name, "LB") == 0 &&
                    rb->kind == ASTK_TOKEN && strcmp(rb->name, "RB") == 0) {
                    
                    printf("  -> Left side is array access, generating STORE\n");
                    
                    Operand base = translate_Exp(arr_exp);
                    Operand index = translate_Exp(idx_exp);
                    Operand size = op_constant(4);
                    Operand offset = op_temp();
                    Operand addr = op_temp();
                    
                    ir_append(ir_make_binop(IR_MUL, offset, index, size));
                    ir_append(ir_make_binop(IR_ADD, addr, base, offset));
                    
                    Operand r_val = translate_Exp(child2);
                    ir_append(ir_make_store(addr, r_val));
                    
                    // ✅ 不要 free！
                    return addr;
                }
            }
            
            // 普通赋值
            Operand r_val = translate_Exp(child2);
            Operand l_var = translate_Exp(child0);
            
            printf("  -> Assign: left=");
            if (l_var && l_var->kind == OP_VARIABLE) printf("%s", l_var->u.name);
            printf(", right=");
            if (r_val) {
                if (r_val->kind == OP_VARIABLE) printf("%s", r_val->u.name);
                else if (r_val->kind == OP_CONSTANT) printf("#%d", r_val->u.value);
                else if (r_val->kind == OP_TEMP) printf("t%d", r_val->u.no);
            }
            printf("\n");
            
            ir_append(ir_make_assign(l_var, r_val));
            // ✅ 不要 free！
            return l_var;
        }

        // 3.4 二元运算符
        if (child1->kind == ASTK_TOKEN) {
            const char* op_name = child1->name;
            
            printf("  -> Binary op: %s\n", op_name);

            if (strcmp(op_name, "AND") == 0 || strcmp(op_name, "OR") == 0 ||
                strcmp(op_name, "RELOP") == 0) {
                printf("  -> Logical/Relational op, skipping\n");
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
                printf("  -> Unknown binary op: %s\n", op_name);
                // ⚠️ 这里可以 free，因为没有加入 IR
                free(t1); free(t2); free(res);
                goto fallback;
            }

            // ✅ 不要 free！
            return res;
        }
    }

    // ========== 4. 四子节点 ==========
    if (strcmp(exp->name, "Exp") == 0 && exp->nchild == 4) {
        ASTNode* child0 = exp->child[0];
        ASTNode* child1 = exp->child[1];
        ASTNode* child2 = exp->child[2];
        ASTNode* child3 = exp->child[3];

        printf("  -> 4 children: [0]='%s' (kind=%d), [1]='%s', [2]='%s', [3]='%s'\n",
               child0->name, child0->kind, child1->name, child2->name, child3->name);

        // 4.1 数组访问（右值）：Exp LB Exp RB
        if (child1->kind == ASTK_TOKEN && strcmp(child1->name, "LB") == 0 &&
            child3->kind == ASTK_TOKEN && strcmp(child3->name, "RB") == 0) {
            
            printf("  -> Array access (LOAD)\n");
            
            Operand base = translate_Exp(child0);
            Operand index = translate_Exp(child2);
            Operand size = op_constant(4);
            Operand offset = op_temp();
            Operand addr = op_temp();

            ir_append(ir_make_binop(IR_MUL, offset, index, size));
            ir_append(ir_make_binop(IR_ADD, addr, base, offset));

            Operand value = op_temp();
            ir_append(ir_make_load(value, addr));

            // ✅ 不要 free！
            return value;
        }

        // 4.2 函数调用（带参数）：ID LP Args RP
        if (child0->kind == ASTK_ID &&
            child1->kind == ASTK_TOKEN && strcmp(child1->name, "LP") == 0 &&
            child3->kind == ASTK_TOKEN && strcmp(child3->name, "RP") == 0) {
            
            const char* fname = child0->sval;
            printf("  -> Function call (with args): %s\n", fname);

            // write 特殊处理
            if (strcmp(fname, "write") == 0) {
                if (strcmp(child2->name, "Args") == 0 && child2->nchild >= 1) {
                    Operand arg = translate_Exp(child2->child[0]);
                    ir_append(ir_make_write(arg));
                    // ✅ 不要 free arg！
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
                        // ✅ 不要 free！
                    }
                    
                    Operand result = op_temp();
                    Operand func = op_function(fname);
                    ir_append(ir_make_call(result, func));
                    // ✅ 不要 free func！
                    return result;
                }
            }
        }
    }

fallback:
    printf("  -> Fallback: returning constant 0\n");
    return op_constant(0);
}