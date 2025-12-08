#include "codegen.h"
#include "mips.h"
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================== 寄存器分配器 ==============================
#define REG_COUNT 10
static const char *reg_names[REG_COUNT] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"
};

static int reg_used[REG_COUNT] = {0};
static int stack_offset = 8;  // 8 开始：0($sp)存旧fp，4($sp)存ra
static char current_func[64] = {0};

typedef struct {
    char name[64];
    int offset;  // 相对于 $fp 的负偏移
} VarInfo;
static VarInfo *var_table = NULL;
static int var_count = 0;

static void regalloc_reset(void) {
    memset(reg_used, 0, sizeof(reg_used));
    stack_offset = 8;
    var_count = 0;
    if (var_table) free(var_table);
    var_table = NULL;
    current_func[0] = '\0';
}

static int get_var_offset(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0)
            return var_table[i].offset;
    }
    var_table = realloc(var_table, (var_count + 1) * sizeof(VarInfo));
    assert(var_table);
    strncpy(var_table[var_count].name, name, 63);
    var_table[var_count].offset = stack_offset;
    stack_offset += 4;
    var_count++;
    return var_table[var_count - 1].offset;
}

static int alloc_reg(void) {
    for (int i = 0; i < REG_COUNT; i++) {
        if (!reg_used[i]) {
            reg_used[i] = 1;
            return i;
        }
    }
    return -1;
}

static void reset_regs(void) {
    memset(reg_used, 0, sizeof(reg_used));
}

static const char* temp_reg(void) {
    static int idx = 0;
    const char *regs[] = {"$t8", "$t9"};
    return regs[idx++ % 2];
}

// 辅助函数：判断是否为立即数并提取值
static int is_immediate(const char *str, int *value) {
    if (str[0] == '#') {
        *value = atoi(str + 1);
        return 1;
    }
    return 0;
}

// 核心函数：获取操作数字符串
// is_dest == 1 表示是赋值目标
static void get_operand(Operand op, char *buf, int is_dest, FILE *out) {
    char name[64] = {0};

    if (!op) { strcpy(buf, "$zero"); return; }

    switch (op->kind) {
        case OP_CONSTANT:
            // 立即数特殊标记，后续处理时识别
            sprintf(buf, "#%d", op->u.value);
            return;
        case OP_VARIABLE:
            strcpy(name, op->u.name);
            break;
        case OP_TEMP:
            sprintf(name, "t%d", op->u.no);
            break;
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

    int r = alloc_reg();
    if (r != -1) {
        sprintf(buf, "%s", reg_names[r]);
        return;
    }

    // 溢出到栈
    int offset = get_var_offset(name);
    if (!is_dest) {
        const char *t = temp_reg();
        emit_lw(out, t, -offset, "$fp");
        sprintf(buf, "%s", t);
    } else {
        sprintf(buf, "-%d($fp)", offset);
    }
}
// ============================== 主生成函数 ==============================
void generate_mips_code(InterCodes head, FILE *out) {
    regalloc_reset();
    mips_init(out);

    InterCodes p = head;
    int frame_size = 0;

    while (p) {
        InterCode c = p->code;
        char x[32] = {0}, y[32] = {0}, z[32] = {0}, label[32] = {0};
        int imm_val;  // 用于存储提取的立即数

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
                reset_regs();
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
                get_operand(c->u.assign.left,  z, 1, out);

                // 检查源操作数是否为立即数（以#开头）
                if (is_immediate(x, &imm_val)) {
                    if (strstr(z, "($fp)")) {
                        // 目标是栈内存：先加载到临时寄存器，再存到内存
                        emit_li(out, "$t8", imm_val);
                        emit_sw(out, "$t8", atoi(z + 1), "$fp");
                    } else {
                        // 目标是寄存器：直接用li指令
                        emit_li(out, z, imm_val);
                    }
                } else {
                    // 源操作数是寄存器，使用move
                    if (strstr(z, "($fp)")) {
                        emit_move(out, "$t8", x);
                        emit_sw(out, "$t8", atoi(z + 1), "$fp");
                    } else {
                        emit_move(out, z, x);
                    }
                }
                break;
            }
            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
                get_operand(c->u.binop.op1, x, 0, out);
                get_operand(c->u.binop.op2, y, 0, out);
                get_operand(c->u.binop.result, z, 1, out);

                const char *dest = strstr(z, "($fp)") ? "$t8" : z;
                char x_reg[32], y_reg[32];
                strcpy(x_reg, x);
                strcpy(y_reg, y);

                // 处理第一个操作数为立即数的情况
                if (is_immediate(x, &imm_val)) {
                    strcpy(x_reg, temp_reg()); // 使用临时寄存器
                    emit_li(out, x_reg, imm_val);
                }

                // 处理第二个操作数为立即数的情况
                if (is_immediate(y, &imm_val)) {
                    strcpy(y_reg, temp_reg()); // 使用临时寄存器
                    emit_li(out, y_reg, imm_val);
                }

                // 生成运算指令
                if (c->kind == IR_ADD) emit_add(out, dest, x_reg, y_reg);
                if (c->kind == IR_SUB) emit_sub(out, dest, x_reg, y_reg);
                if (c->kind == IR_MUL) emit_mul(out, dest, x_reg, y_reg);
                if (c->kind == IR_DIV) emit_div(out, dest, x_reg, y_reg);

                if (strstr(z, "($fp)"))
                    emit_sw(out, "$t8", atoi(z + 1), "$fp");
                break;
            }

            case IR_CALL: {
                get_operand(c->u.call.result, z, 1, out);
                emit_jal(out, c->u.call.func->u.name);
                if (strstr(z, "($fp)")) {
                    emit_sw(out, "$v0", atoi(z + 1), "$fp");
                } else {
                    emit_move(out, z, "$v0");
                }
                break;
            }

            case IR_RETURN: {
                get_operand(c->u.one.x, x, 0, out);
                if (is_immediate(x, &imm_val)) {
                    emit_li(out, "$v0", imm_val);
                } else {
                    emit_move(out, "$v0", x);
                }
                if (frame_size > 8) {
                    emit_lw(out, "$ra", frame_size - 4, "$sp");
                    emit_lw(out, "$fp", frame_size - 8, "$sp");
                    emit_addi(out, "$sp", "$sp", frame_size);
                }
                emit_jr(out, "$ra");
                break;
            }

            case IR_ARG: {
                static int arg_idx = 0;
                get_operand(c->u.one.x, x, 0, out);
                
                // 处理参数为立即数的情况
                if (is_immediate(x, &imm_val)) {
                    if (arg_idx < 4) {
                        char a[8]; sprintf(a, "$a%d", arg_idx);
                        emit_li(out, a, imm_val);
                    } else {
                        int off = (arg_idx - 4) * 4;
                        emit_li(out, "$t8", imm_val);
                        emit_sw(out, "$t8", off + frame_size, "$sp");
                    }
                } else {
                    if (arg_idx < 4) {
                        char a[8]; sprintf(a, "$a%d", arg_idx);
                        emit_move(out, a, x);
                    } else {
                        int off = (arg_idx - 4) * 4;
                        emit_sw(out, x, off + frame_size, "$sp");
                    }
                }
                arg_idx++;
                break;
            }

            case IR_PARAM:
                get_var_offset(c->u.one.x->u.name);
                break;

            case IR_READ:
                get_operand(c->u.one.x, z, 1, out);
                emit_jal(out, "read");
                if (strstr(z, "($fp)"))
                    emit_sw(out, "$v0", atoi(z + 1), "$fp");
                else
                    emit_move(out, z, "$v0");
                break;

            case IR_WRITE:
                get_operand(c->u.one.x, x, 0, out);
                
                // 处理输出立即数的情况
                if (is_immediate(x, &imm_val)) {
                    emit_li(out, "$a0", imm_val);
                } else {
                    emit_move(out, "$a0", x);
                }
                emit_jal(out, "write");
                break;

            case IR_IF_GOTO: {
                get_operand(c->u.if_goto.x, x, 0, out);
                get_operand(c->u.if_goto.y, y, 0, out);
                sprintf(label, "label%d", c->u.if_goto.label->u.no);
                const char *rel = c->u.if_goto.relop;

                // 处理比较操作数中的立即数
                char x_reg[32], y_reg[32];
                strcpy(x_reg, x);
                strcpy(y_reg, y);

                if (is_immediate(x, &imm_val)) {
                    strcpy(x_reg, temp_reg());
                    emit_li(out, x_reg, imm_val);
                }
                if (is_immediate(y, &imm_val)) {
                    strcpy(y_reg, temp_reg());
                    emit_li(out, y_reg, imm_val);
                }

                if (strcmp(rel, "==") == 0) emit_beq(out, x_reg, y_reg, label);
                else if (strcmp(rel, "!=") == 0) emit_bne(out, x_reg, y_reg, label);
                else if (strcmp(rel, "<")  == 0) emit_blt(out, x_reg, y_reg, label);
                else if (strcmp(rel, ">")  == 0) emit_bgt(out, x_reg, y_reg, label);
                else if (strcmp(rel, "<=") == 0) emit_ble(out, x_reg, y_reg, label);
                else if (strcmp(rel, ">=") == 0) emit_bge(out, x_reg, y_reg, label);
                break;
            }

            case IR_DEC: {
                char base[64];
                if (c->u.dec.x->kind == OP_VARIABLE)
                    strcpy(base, c->u.dec.x->u.name);
                else
                    sprintf(base, "t%d", c->u.dec.x->u.no);
                for (int i = 0; i < c->u.dec.size; i += 4) {
                    char name[70];
                    sprintf(name, "%s_%d", base, i);
                    get_var_offset(name);
                }
                break;
            }

            case IR_LOAD: {
                get_operand(c->u.load.right, x, 0, out);  // addr
                get_operand(c->u.load.left,  z, 1, out);  // result
                
                // 处理地址为立即数的情况
                char addr_reg[32];
                strcpy(addr_reg, x);
                if (is_immediate(x, &imm_val)) {
                    strcpy(addr_reg, temp_reg());
                    emit_li(out, addr_reg, imm_val);
                }

                if (strstr(z, "($fp)")) {
                    emit_lw(out, "$t8", 0, addr_reg);
                    emit_sw(out, "$t8", atoi(z + 1), "$fp");
                } else {
                    emit_lw(out, z, 0, addr_reg);
                }
                break;
            }

            case IR_STORE: {
                get_operand(c->u.store.addr,  x, 0, out);
                get_operand(c->u.store.value, y, 0, out);
                
                // 处理地址和值为立即数的情况
                char addr_reg[32], val_reg[32];
                strcpy(addr_reg, x);
                strcpy(val_reg, y);

                if (is_immediate(x, &imm_val)) {
                    strcpy(addr_reg, temp_reg());
                    emit_li(out, addr_reg, imm_val);
                }
                if (is_immediate(y, &imm_val)) {
                    strcpy(val_reg, temp_reg());
                    emit_li(out, val_reg, imm_val);
                }

                emit_sw(out, val_reg, 0, addr_reg);
                break;
            }

            default:
                break;
        }
        p = p->next;
    }

    emit_label(out, "exit");
    emit_li(out, "$v0", 10);
    emit_syscall(out);

    if (var_table) free(var_table);
}

void generate_mips(InterCodes head, FILE *out) {
    generate_mips_code(head, out);
}