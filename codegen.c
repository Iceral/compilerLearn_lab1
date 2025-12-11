#include "codegen.h"
#include "mips.h"
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================== 全局变量 ==============================
static int stack_offset = 0;
static char current_func[64] = {0};
static int current_frame_size = 0;

static int g_param_idx = 0;

typedef struct {
    char name[64];
    int offset;
} VarInfo;

static VarInfo *var_table = NULL;
static int var_count = 0;
static int var_capacity = 0;

// ============================== 辅助函数 ==============================

static void regalloc_reset(void) {
    stack_offset = 0;
    var_count = 0;
    if (var_table) {
        free(var_table);
        var_table = NULL;
    }
    var_capacity = 0;
    g_param_idx = 0;
}

static int find_var_offset(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0)
            return var_table[i].offset;
    }
    return -1;
}

static int alloc_var_size(const char *name, int size) {
    int existing = find_var_offset(name);
    if (existing >= 0) return existing;
    
    if (var_count >= var_capacity) {
        var_capacity = var_capacity == 0 ? 32 : var_capacity * 2;
        var_table = realloc(var_table, var_capacity * sizeof(VarInfo));
        assert(var_table);
    }
    
    strncpy(var_table[var_count].name, name, 63);
    var_table[var_count].name[63] = '\0';
    
    if (stack_offset == 0) stack_offset = 12;
    
    var_table[var_count].offset = stack_offset;
    stack_offset += size;
    var_count++;
    
    return var_table[var_count - 1].offset;
}

static int get_var_offset(const char *name) {
    return alloc_var_size(name, 4);
}

static const char *temp_reg(int index) {
    return (index == 0) ? "$t8" : "$t9";
}

static int is_immediate(const char *str, int *value) {
    if (str && str[0] == '#') {
        *value = atoi(str + 1);
        return 1;
    }
    return 0;
}

static int is_memory(const char *op_str) {
    return op_str && strstr(op_str, "($fp)") != NULL;
}

static int parse_offset(const char *op_str) {
    if (!op_str) return 0;
    return atoi(op_str);
}

static void get_operand(Operand op, char *buf, int is_dest, FILE *out) {
    char name[64] = {0};

    if (!op) {
        strcpy(buf, "$zero");
        return;
    }

    switch (op->kind) {
        case OP_CONSTANT:
            sprintf(buf, "#%d", op->u.value);
            return;
        case OP_VARIABLE: {
            int offset = get_var_offset(op->u.name);
            sprintf(buf, "-%d($fp)", offset);
            return;
        }
        case OP_TEMP: {
            sprintf(name, "t%d", op->u.no);
            int offset = get_var_offset(name);
            sprintf(buf, "-%d($fp)", offset);
            return;
        }
        case OP_ADDRESS:
            get_operand(op->u.base, buf, 0, out);
            return;
        case OP_LABEL:
            sprintf(buf, "label%d", op->u.no);
            return;
        case OP_FUNCTION:
            strcpy(buf, op->u.name);
            return;
        default:
            strcpy(buf, "$zero");
            return;
    }
}

// ★★★ 新增：加载操作数到寄存器，正确处理 OP_ADDRESS ★★★
static void load_operand(Operand op, const char *reg, FILE *out) {
    char buf[64];
    int imm_val;
    
    if (!op) {
        emit_li(out, reg, 0);
        return;
    }
    
    if (op->kind == OP_ADDRESS) {
        // &var -> 计算地址，不是加载值
        get_operand(op->u.base, buf, 0, out);
        int offset = parse_offset(buf);
        emit_addi(out, reg, "$fp", offset);
    } else if (op->kind == OP_CONSTANT) {
        emit_li(out, reg, op->u.value);
    } else {
        get_operand(op, buf, 0, out);
        if (is_immediate(buf, &imm_val)) {
            emit_li(out, reg, imm_val);
        } else if (is_memory(buf)) {
            emit_lw(out, reg, parse_offset(buf), "$fp");
        }
    }
}

// ============================== 预扫描 ==============================

static int compute_frame_size(InterCodes start) {
    if (!start) return 16;
    
    InterCodes p = start->next;
    int max_iter = 100000;
    int iter = 0;
    
    while (p && p->code->kind != IR_FUNCTION && iter < max_iter) {
        InterCode c = p->code;
        iter++;
        char name[64];
        
        switch (c->kind) {
            case IR_DEC: {
                if (c->u.dec.x->kind == OP_VARIABLE)
                    strcpy(name, c->u.dec.x->u.name);
                else
                    sprintf(name, "t%d", c->u.dec.x->u.no);
                alloc_var_size(name, c->u.dec.size);
                break;
            }
            
            case IR_ASSIGN:
                if (c->u.assign.left) {
                    if (c->u.assign.left->kind == OP_VARIABLE)
                        get_var_offset(c->u.assign.left->u.name);
                    else if (c->u.assign.left->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.assign.left->u.no);
                        get_var_offset(name);
                    }
                }
                if (c->u.assign.right) {
                    if (c->u.assign.right->kind == OP_VARIABLE)
                        get_var_offset(c->u.assign.right->u.name);
                    else if (c->u.assign.right->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.assign.right->u.no);
                        get_var_offset(name);
                    }
                    else if (c->u.assign.right->kind == OP_ADDRESS) {
                        Operand base = c->u.assign.right->u.base;
                        if (base->kind == OP_VARIABLE)
                            get_var_offset(base->u.name);
                        else if (base->kind == OP_TEMP) {
                            sprintf(name, "t%d", base->u.no);
                            get_var_offset(name);
                        }
                    }
                }
                break;
                
            case IR_LOAD:
                if (c->u.load.left) {
                    if (c->u.load.left->kind == OP_VARIABLE)
                        get_var_offset(c->u.load.left->u.name);
                    else if (c->u.load.left->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.load.left->u.no);
                        get_var_offset(name);
                    }
                }
                if (c->u.load.right) {
                    if (c->u.load.right->kind == OP_VARIABLE)
                        get_var_offset(c->u.load.right->u.name);
                    else if (c->u.load.right->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.load.right->u.no);
                        get_var_offset(name);
                    }
                }
                break;
                
            case IR_STORE:
                if (c->u.store.addr) {
                    if (c->u.store.addr->kind == OP_VARIABLE)
                        get_var_offset(c->u.store.addr->u.name);
                    else if (c->u.store.addr->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.store.addr->u.no);
                        get_var_offset(name);
                    }
                }
                if (c->u.store.value) {
                    if (c->u.store.value->kind == OP_VARIABLE)
                        get_var_offset(c->u.store.value->u.name);
                    else if (c->u.store.value->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.store.value->u.no);
                        get_var_offset(name);
                    }
                }
                break;
                
            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
                Operand ops[] = {c->u.binop.result, c->u.binop.op1, c->u.binop.op2};
                for (int i = 0; i < 3; i++) {
                    if (ops[i]) {
                        if (ops[i]->kind == OP_VARIABLE)
                            get_var_offset(ops[i]->u.name);
                        else if (ops[i]->kind == OP_TEMP) {
                            sprintf(name, "t%d", ops[i]->u.no);
                            get_var_offset(name);
                        }
                        else if (ops[i]->kind == OP_ADDRESS) {
                            Operand base = ops[i]->u.base;
                            if (base->kind == OP_VARIABLE)
                                get_var_offset(base->u.name);
                            else if (base->kind == OP_TEMP) {
                                sprintf(name, "t%d", base->u.no);
                                get_var_offset(name);
                            }
                        }
                    }
                }
                break;
            }
            
            case IR_READ:
            case IR_WRITE:
            case IR_ARG:
            case IR_RETURN:
                if (c->u.one.x) {
                    if (c->u.one.x->kind == OP_VARIABLE)
                        get_var_offset(c->u.one.x->u.name);
                    else if (c->u.one.x->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.one.x->u.no);
                        get_var_offset(name);
                    }
                    else if (c->u.one.x->kind == OP_ADDRESS) {
                        Operand base = c->u.one.x->u.base;
                        if (base->kind == OP_VARIABLE)
                            get_var_offset(base->u.name);
                        else if (base->kind == OP_TEMP) {
                            sprintf(name, "t%d", base->u.no);
                            get_var_offset(name);
                        }
                    }
                }
                break;
                
            case IR_CALL:
                if (c->u.call.result) {
                    if (c->u.call.result->kind == OP_VARIABLE)
                        get_var_offset(c->u.call.result->u.name);
                    else if (c->u.call.result->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.call.result->u.no);
                        get_var_offset(name);
                    }
                }
                break;
                
            case IR_PARAM:
                if (c->u.one.x && c->u.one.x->kind == OP_VARIABLE)
                    get_var_offset(c->u.one.x->u.name);
                break;
                
            case IR_IF_GOTO:
                if (c->u.if_goto.x) {
                    if (c->u.if_goto.x->kind == OP_VARIABLE)
                        get_var_offset(c->u.if_goto.x->u.name);
                    else if (c->u.if_goto.x->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.if_goto.x->u.no);
                        get_var_offset(name);
                    }
                }
                if (c->u.if_goto.y) {
                    if (c->u.if_goto.y->kind == OP_VARIABLE)
                        get_var_offset(c->u.if_goto.y->u.name);
                    else if (c->u.if_goto.y->kind == OP_TEMP) {
                        sprintf(name, "t%d", c->u.if_goto.y->u.no);
                        get_var_offset(name);
                    }
                }
                break;
                
            default:
                break;
        }
        p = p->next;
    }
    
    // ★★★ 关键修复：额外预留 8 字节给 $ra 和 old_$fp ★★★
    int frame_size = stack_offset + 8;
    
    // 对齐到 8 字节
    if (frame_size % 8 != 0) frame_size += 4;
    if (frame_size < 16) frame_size = 16;
    
    return frame_size;
}

static int collect_args(InterCodes call_node, InterCodes *args, int max_args) {
    int count = 0;
    InterCodes q = call_node->prev;
    
    // 直接按顺序存储，靠近 CALL 的 ARG 是第一个参数
    while (q && q->code->kind == IR_ARG && count < max_args) {
        args[count] = q;
        count++;
        q = q->prev;
    }
    
    return count;
}

// ============================== 主生成函数 ==============================

void generate_mips_code(InterCodes head, FILE *out) {
    if (!head) return;
    
    regalloc_reset();
    mips_init(out);

    InterCodes p = head;

    while (p) {
        if (!p->code) {
            p = p->next;
            continue;
        }
        
        InterCode c = p->code;
        char x[64] = {0}, y[64] = {0}, z[64] = {0}, label[64] = {0};
        int imm_val;

        switch (c->kind) {
            case IR_FUNCTION: {
                char func_name[64];
                strcpy(func_name, c->u.one.x->u.name);
                
                regalloc_reset();
                strcpy(current_func, func_name);
                current_frame_size = compute_frame_size(p);
                
                fprintf(out, "\n.globl %s\n", current_func);
                emit_label(out, current_func);

                if (current_frame_size > 0) {
                    emit_addi(out, "$sp", "$sp", -current_frame_size);
                    emit_sw(out, "$ra", 0, "$sp");
                    emit_sw(out, "$fp", 4, "$sp");
                    emit_addi(out, "$fp", "$sp", current_frame_size);
                }
                break;
            }

            case IR_LABEL:
                sprintf(label, "label%d", c->u.one.x->u.no);
                emit_label(out, label);
                break;

            case IR_GOTO:
                sprintf(label, "label%d", c->u.one.x->u.no);
                emit_j(out, label);
                break;

            case IR_ASSIGN: {
                if (c->u.assign.right->kind == OP_ADDRESS) {
                    get_operand(c->u.assign.left, z, 1, out);
                    get_operand(c->u.assign.right->u.base, y, 0, out);
                    int offset = parse_offset(y);
                    emit_addi(out, temp_reg(0), "$fp", offset);
                    if (is_memory(z)) {
                        emit_sw(out, temp_reg(0), parse_offset(z), "$fp");
                    }
                    break;
                }

                get_operand(c->u.assign.right, x, 0, out);
                get_operand(c->u.assign.left, z, 1, out);

                if (is_immediate(x, &imm_val)) {
                    emit_li(out, temp_reg(0), imm_val);
                    if (is_memory(z)) {
                        emit_sw(out, temp_reg(0), parse_offset(z), "$fp");
                    }
                } else if (is_memory(x)) {
                    emit_lw(out, temp_reg(0), parse_offset(x), "$fp");
                    if (is_memory(z)) {
                        emit_sw(out, temp_reg(0), parse_offset(z), "$fp");
                    }
                }
                break;
            }

            // ★★★ 关键修改：使用 load_operand 正确处理 OP_ADDRESS ★★★
            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
                Operand op1 = c->u.binop.op1;
                Operand op2 = c->u.binop.op2;
                
                get_operand(c->u.binop.result, z, 1, out);

                const char *rx = temp_reg(0);
                const char *ry = temp_reg(1);
                
                // 使用 load_operand 正确处理 OP_ADDRESS
                load_operand(op1, rx, out);
                load_operand(op2, ry, out);
                
                if (c->kind == IR_ADD) emit_add(out, rx, rx, ry);
                else if (c->kind == IR_SUB) emit_sub(out, rx, rx, ry);
                else if (c->kind == IR_MUL) emit_mul(out, rx, rx, ry);
                else if (c->kind == IR_DIV) emit_div(out, rx, rx, ry);
                
                if (is_memory(z)) {
                    emit_sw(out, rx, parse_offset(z), "$fp");
                }
                break;
            }

            case IR_CALL: {
                InterCodes args[32];
                int arg_count = collect_args(p, args, 32);
                
                for (int i = 0; i < arg_count; i++) {
                    Operand arg_op = args[i]->code->u.one.x;
                    
                    // 使用 load_operand 处理参数
                    load_operand(arg_op, temp_reg(0), out);
                    
                    if (i < 4) {
                        char a[8];
                        sprintf(a, "$a%d", i);
                        emit_move(out, a, temp_reg(0));
                    } else {
                        emit_addi(out, "$sp", "$sp", -4);
                        emit_sw(out, temp_reg(0), 0, "$sp");
                    }
                }
                
                get_operand(c->u.call.result, z, 1, out);
                emit_jal(out, c->u.call.func->u.name);
                
                if (arg_count > 4) {
                    emit_addi(out, "$sp", "$sp", (arg_count - 4) * 4);
                }
                
                if (is_memory(z)) {
                    emit_sw(out, "$v0", parse_offset(z), "$fp");
                }
                break;
            }

            case IR_ARG:
                break;

            case IR_RETURN: {
                load_operand(c->u.one.x, "$v0", out);

                if (current_frame_size > 0) {
                    emit_lw(out, "$ra", 0, "$sp");
                    emit_lw(out, "$fp", 4, "$sp");
                    emit_addi(out, "$sp", "$sp", current_frame_size);
                }
                emit_jr(out, "$ra");
                break;
            }

            case IR_PARAM: {
                if (!p->prev || (p->prev->code->kind != IR_PARAM && p->prev->code->kind != IR_FUNCTION)) {
                    g_param_idx = 0;
                }
                
                int offset = get_var_offset(c->u.one.x->u.name);
                
                if (g_param_idx < 4) {
                    fprintf(out, "    sw $a%d, -%d($fp)\n", g_param_idx, offset);
                } else {
                    int caller_offset = (g_param_idx - 4) * 4;
                    emit_lw(out, temp_reg(0), caller_offset, "$fp");
                    emit_sw(out, temp_reg(0), -offset, "$fp");
                }
                g_param_idx++;
                break;
            }

            case IR_READ:
                get_operand(c->u.one.x, z, 1, out);
                emit_addi(out, "$sp", "$sp", -4);
                emit_sw(out, "$ra", 0, "$sp");
                emit_jal(out, "read");
                emit_lw(out, "$ra", 0, "$sp");
                emit_addi(out, "$sp", "$sp", 4);
                if (is_memory(z))
                    emit_sw(out, "$v0", parse_offset(z), "$fp");
                break;

            case IR_WRITE:
                load_operand(c->u.one.x, "$a0", out);
                emit_addi(out, "$sp", "$sp", -4);
                emit_sw(out, "$ra", 0, "$sp");
                emit_jal(out, "write");
                emit_lw(out, "$ra", 0, "$sp");
                emit_addi(out, "$sp", "$sp", 4);
                break;

            case IR_IF_GOTO: {
                sprintf(label, "label%d", c->u.if_goto.label->u.no);
                
                const char *rx = temp_reg(0);
                const char *ry = temp_reg(1);

                load_operand(c->u.if_goto.x, rx, out);
                load_operand(c->u.if_goto.y, ry, out);

                const char *rel = c->u.if_goto.relop;
                if (strcmp(rel, "==") == 0) emit_beq(out, rx, ry, label);
                else if (strcmp(rel, "!=") == 0) emit_bne(out, rx, ry, label);
                else if (strcmp(rel, "<") == 0) emit_blt(out, rx, ry, label);
                else if (strcmp(rel, ">") == 0) emit_bgt(out, rx, ry, label);
                else if (strcmp(rel, "<=") == 0) emit_ble(out, rx, ry, label);
                else if (strcmp(rel, ">=") == 0) emit_bge(out, rx, ry, label);
                break;
            }

            case IR_DEC:
                break;

            case IR_LOAD: {
                get_operand(c->u.load.left, z, 1, out);
                
                // 加载地址
                load_operand(c->u.load.right, temp_reg(0), out);
                
                // 从地址读取值
                emit_lw(out, temp_reg(1), 0, temp_reg(0));
                
                if (is_memory(z))
                    emit_sw(out, temp_reg(1), parse_offset(z), "$fp");
                break;
            }

            case IR_STORE: {
                // 加载目标地址
                load_operand(c->u.store.addr, temp_reg(0), out);
                
                // 加载要存储的值
                load_operand(c->u.store.value, temp_reg(1), out);
                
                // 存储到地址
                emit_sw(out, temp_reg(1), 0, temp_reg(0));
                break;
            }

            default:
                break;
        }
        
        p = p->next;
    }
    
    if (var_table) {
        free(var_table);
        var_table = NULL;
    }
    var_count = 0;
    var_capacity = 0;
}

void generate_mips(InterCodes head, FILE *out) {
    generate_mips_code(head, out);
}
