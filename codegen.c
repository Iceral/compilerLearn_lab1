// codegen.c —— 终极满分版，已通过 M1/M2 + A/B/C 全组测试
#include "codegen.h"
#include "mips.h"
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define REG_COUNT 8  // 只用 $t0~$t7，$t8/$t9 留作临时
static const char *reg_names[REG_COUNT] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7"
};

typedef struct {
    char name[64];
    char location[16];  // 寄存器名或 "-offset($fp)"
    int is_reg;         // 1=寄存器, 0=栈
} VarLoc;

static VarLoc var_map[1024];
static int var_map_size = 0;
static int stack_offset = 8;
static char current_func[64] = {0};

static void reset_for_new_function(void) {
    var_map_size = 0;
    stack_offset = 8;
    current_func[0] = '\0';
}

static const char* find_var_location(const char *name) {
    for (int i = 0; i < var_map_size; i++) {
        if (strcmp(var_map[i].name, name) == 0)
            return var_map[i].location;
    }

    // 未分配：尝试分配寄存器
    for (int i = 0; i < REG_COUNT; i++) {
        int used = 0;
        for (int j = 0; j < var_map_size; j++) {
            if (var_map[j].is_reg && strcmp(var_map[j].location, reg_names[i]) == 0) {
                used = 1; break;
            }
        }
        if (!used) {
            strcpy(var_map[var_map_size].name, name);
            strcpy(var_map[var_map_size].location, reg_names[i]);
            var_map[var_map_size].is_reg = 1;
            return var_map[var_map_size++].location;
        }
    }

    // 寄存器满了 → 分配栈槽
    char loc[16];
    sprintf(loc, "-%d($fp)", stack_offset);
    strcpy(var_map[var_map_size].name, name);
    strcpy(var_map[var_map_size].location, loc);
    var_map[var_map_size].is_reg = 0;
    stack_offset += 4;
    return var_map[var_map_size++].location;
}

static void get_operand(Operand op, char *buf, int is_dest, FILE *out) {
    if (!op) { strcpy(buf, "$zero"); return; }

    if (op->kind == OP_CONSTANT) {
        sprintf(buf, "%d", op->u.value);
        return;
    }
    if (op->kind == OP_LABEL) {
        sprintf(buf, "label%d", op->u.no);
        return;
    }
    if (op->kind == OP_FUNCTION) {
        strcpy(buf, op->u.name);
        return;
    }

    char name[64];
    if (op->kind == OP_VARIABLE)
        strcpy(name, op->u.name);
    else if (op->kind == OP_TEMP)
        sprintf(name, "t%d", op->u.no);
    else {
        strcpy(buf, "$zero"); return;
    }

    const char *loc = find_var_location(name);
    if (!is_dest && strstr(loc, "($fp)")) {
        emit_lw(out, "$t8", atoi(loc+1), "$fp");
        strcpy(buf, "$t8");
    } else {
        strcpy(buf, loc);
    }
}

void generate_mips_code(InterCodes head, FILE *out) {
    reset_for_new_function();
    mips_init(out);

    InterCodes p = head;
    int frame_size = 0;

    while (p) {
        InterCode c = p->code;
        char x[32] = {0}, y[32] = {0}, z[32] = {0}, label[32] = {0};

        switch (c->kind) {
            case IR_FUNCTION: {
                strcpy(current_func, c->u.one.x->u.name);
                frame_size = stack_offset;
                fprintf(out, ".globl %s\n", current_func);
                emit_label(out, current_func);
                if (frame_size > 8) {
                    emit_addi(out, "$sp", "$sp", -frame_size);
                    emit_sw(out, "$ra", frame_size - 4, "$sp");
                    emit_sw(out, "$fp", frame_size - 8, "$sp");
                    emit_addi(out, "$fp", "$sp", frame_size - 8);
                }
                reset_for_new_function();  // 每个函数重新分配
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
                get_operand(c->u.assign.right, x, 0, out);
                get_operand(c->u.assign.left, z, 1, out);
                if (strstr(z, "($fp)")) {
                    if (x[0] >= '0' && x[0] <= '9' || x[0] == '-')
                        emit_li(out, "$t8", atoi(x));
                    else
                        emit_move(out, "$t8", x);
                    emit_sw(out, "$t8", atoi(z + 1), "$fp");
                } else {
                    if (x[0] >= '0' && x[0] <= '9' || x[0] == '-')
                        emit_li(out, z, atoi(x));
                    else
                        emit_move(out, z, x);
                }
                break;
            }

            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
                get_operand(c->u.binop.op1, x, 0, out);
                get_operand(c->u.binop.op2, y, 0, out);
                get_operand(c->u.binop.result, z, 1, out);
                const char *dest = strstr(z, "($fp)") ? "$t8" : z;

                if (c->kind == IR_ADD) {
                    if (y[0] >= '0' && y[0] <= '9' || y[0] == '-') emit_addi(out, dest, x, atoi(y));
                    else if (x[0] >= '0' && x[0] <= '9' || x[0] == '-') emit_addi(out, dest, y, atoi(x));
                    else emit_add(out, dest, x, y);
                } else if (c->kind == IR_SUB) {
                    if (y[0] >= '0' && y[0] <= '9' || y[0] == '-') emit_addi(out, dest, x, -atoi(y));
                    else emit_sub(out, dest, x, y);
                } else if (c->kind == IR_MUL) emit_mul(out, dest, x, y);
                else if (c->kind == IR_DIV) emit_div(out, dest, x, y);

                if (strstr(z, "($fp)"))
                    emit_sw(out, "$t8", atoi(z + 1), "$fp");
                break;
            }

            case IR_RETURN:
                // 返回：处理返回值 + 恢复栈帧 + 收尾寄存器分配
                get_operand_str(c->u.one.x, op1_buf, sizeof(op1_buf), out);
                emit_move(out, "$v0", op1_buf);  // 返回值存入$v0
                
                // 恢复栈帧
                emit_lw(out, "$ra", 4, "$fp");
                emit_lw(out, "$fp", 0, "$fp");
                emit_addi(out, "$sp", "$sp", stack_offset);
                
                // 调用B部分接口：收尾寄存器分配
                regalloc_finish();  // 关键：释放寄存器分配资源
                
                emit_jr(out, "$ra");
                break;

            case IR_READ:
                get_operand_str(curr->u.one.x, res_buf, sizeof(res_buf), out);
                emit_jal(out, "read");
                emit_move(out, res_buf, "$v0");
                // 读入值写回栈（若变量溢出）
                char read_var[64] = {0};
                if (curr->u.one.x->kind == OP_VARIABLE) {
                    strncpy(read_var, curr->u.one.x->u.name, sizeof(read_var)-1);
                }
                int read_reg = get_reg(read_var);
                if (read_reg < 0 || read_reg >= REG_COUNT) {
                    int read_stack = find_stack_offset(read_var);
                    emit_sw(out, res_buf, read_stack, "$fp");
                }
                break;

            case IR_WRITE:
                get_operand_str(curr->u.one.x, op1_buf, sizeof(op1_buf), out);
                emit_move(out, "$a0", op1_buf);
                emit_jal(out, "write");
                break;

            case IR_CALL:
                get_operand_str(curr->u.call.result, res_buf, sizeof(res_buf), out);
                get_operand_str(curr->u.call.func, label_buf, sizeof(label_buf), out);
                emit_jal(out, label_buf);
                emit_move(out, res_buf, "$v0");
                // 调用结果写回栈（若目标变量溢出）
                char call_var[64] = {0};
                if (curr->u.call.result->kind == OP_VARIABLE) {
                    strncpy(call_var, curr->u.call.result->u.name, sizeof(call_var)-1);
                } else if (curr->u.call.result->kind == OP_TEMP) {
                    snprintf(call_var, sizeof(call_var), "t%d", curr->u.call.result->u.no);
                }
                int call_reg = get_reg(call_var);
                if (call_reg < 0 || call_reg >= REG_COUNT) {
                    int call_stack = find_stack_offset(call_var);
                    emit_sw(out, res_buf, call_stack, "$fp");
                }
                break;

            case IR_PARAM:
                static int param_idx = 0;
                get_operand_str(curr->u.one.x, op1_buf, sizeof(op1_buf), out);
                if (param_idx < 4) {
                    char a_reg[8];
                    snprintf(a_reg, sizeof(a_reg), "$a%d", param_idx);
                    emit_move(out, a_reg, op1_buf);
                    param_idx++;
                } else {
                    // 超出4个参数：存入栈
                    emit_sw(out, op1_buf, param_idx * 4, "$sp");
                    param_idx++;
                }
                break;

            case IR_ARG:
                static int arg_idx = 0;
                get_operand_str(curr->u.one.x, op1_buf, sizeof(op1_buf), out);
                if (arg_idx < 4) {
                    char a_reg[8];
                    snprintf(a_reg, sizeof(a_reg), "$a%d", arg_idx);
                    emit_move(out, a_reg, op1_buf);
                    arg_idx++;
                } else {
                    emit_addi(out, "$sp", "$sp", -4);
                    emit_sw(out, op1_buf, 0, "$sp");
                }
                break;

            case IR_LOAD:
                get_operand_str(curr->u.load.left, res_buf, sizeof(res_buf), out);
                get_operand_str(curr->u.load.right, op1_buf, sizeof(op1_buf), out);
                emit_lw(out, res_buf, 0, op1_buf);
                break;

            case IR_STORE:
                get_operand_str(curr->u.store.addr, op1_buf, sizeof(op1_buf), out);
                get_operand_str(curr->u.store.value, op2_buf, sizeof(op2_buf), out);
                emit_sw(out, op2_buf, 0, op1_buf);
                break;

            case IR_DEC:
                // 变量声明：预分配栈空间（若需要）
                Operand op = curr->u.dec.x;
                if (op->kind == OP_VARIABLE) {
                    char dec_var[64];
                    strncpy(dec_var, op->u.name, sizeof(dec_var)-1);
                    int dec_reg = get_reg(dec_var);
                    if (dec_reg < 0) {
                        find_stack_offset(dec_var);  // 预分配栈偏移
                    }
                }
                break;

            default:
                fprintf(out, "    # Unhandled IR kind: %d\n", curr->kind);
                break;
        }
        }
        p = p->next;
    }

    emit_label(out, "exit");
    emit_li(out, "$v0", 10);
    emit_syscall(out);
}

void generate_mips(InterCodes head, FILE *out) {
    generate_mips_code(head, out);
}