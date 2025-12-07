#include "codegen.h"
#include "mips.h"
#include "regalloc.h"
#include "ir.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// 寄存器名映射（与MIPS32临时寄存器对应）
static const char *reg_names[] = {
    "$t0", "$t1", "$t2", "$t3", "$t4",
    "$t5", "$t6", "$t7", "$t8", "$t9"
};
#define REG_COUNT (sizeof(reg_names)/sizeof(reg_names[0]))

// 栈偏移管理（处理寄存器溢出）
#define STACK_BASE_OFFSET 4  // 栈基址偏移（$fp为栈帧指针）
static int stack_offset = STACK_BASE_OFFSET;  // 从4开始（跳过$ra）
typedef struct {
    char var_name[64];       // 变量/临时变量名
    int stack_offset;        // 栈偏移（相对于$fp）
} StackMap;
static StackMap *stack_map = NULL;
static int stack_map_size = 0;

// 核心修改：使用B部分get_reg接口获取寄存器（支持栈溢出）
static void get_operand_str(Operand op, char *buf, int buf_len, FILE *out) {
    assert(op != NULL && buf != NULL && buf_len > 0 && out != NULL);
    memset(buf, 0, buf_len);

    char var_name[64] = {0};
    int reg_idx = -1;
    int stack_off = -1;

    switch (op->kind) {
        case OP_VARIABLE: {
            // 1. 获取变量名
            strncpy(var_name, op->u.name, sizeof(var_name)-1);
            // 2. 调用B部分接口分配寄存器
            reg_idx = get_reg(var_name);
            if (reg_idx >= 0 && reg_idx < REG_COUNT) {
                // 分配到寄存器：直接使用
                strncpy(buf, reg_names[reg_idx], buf_len-1);
            } else {
                // 寄存器溢出：分配栈偏移并加载/存储
                stack_off = find_stack_offset(var_name);
                snprintf(buf, buf_len, "-%d($fp)", stack_off);
            }
            break;
        }
        case OP_TEMP: {
            // 临时变量命名：t1/t2/...
            snprintf(var_name, sizeof(var_name), "t%d", op->u.no);
            reg_idx = get_reg(var_name);
            if (reg_idx >= 0 && reg_idx < REG_COUNT) {
                strncpy(buf, reg_names[reg_idx], buf_len-1);
            } else {
                stack_off = find_stack_offset(var_name);
                snprintf(buf, buf_len, "-%d($fp)", stack_off);
            }
            break;
        }
        case OP_CONSTANT: {
            snprintf(buf, buf_len, "%d", op->u.value);
            break;
        }
        case OP_LABEL: {
            snprintf(buf, buf_len, "label%d", op->u.no);
            break;
        }
        case OP_FUNCTION: {
            strncpy(buf, op->u.name, buf_len-1);
            break;
        }
        case OP_ADDRESS: {
            // 地址操作数：&var → 变量的寄存器/栈地址
            get_operand_str(op->u.base, buf, buf_len, out);
            break;
        }
        default: {
            strncpy(buf, "$zero", buf_len-1);
            break;
        }
    }

    // 溢出处理：若变量在栈中，使用前先加载到临时寄存器（仅针对运算操作数）
    if (stack_off != -1 && (op->kind == OP_VARIABLE || op->kind == OP_TEMP)) {
        char temp_reg[8] = "$t9";  // 临时寄存器用于栈加载
        // 加载栈数据到临时寄存器（避免运算直接操作栈）
        emit_lw(out, temp_reg, stack_off, "$fp");
        strncpy(buf, temp_reg, buf_len-1);
    }
}

// 关系运算符映射（保持不变）
static void map_relop_to_binop(const char *relop, char *binop, int buf_len) {
    assert(relop != NULL && binop != NULL && buf_len > 0);
    if (strcmp(relop, "==") == 0) strncpy(binop, "beq", buf_len-1);
    else if (strcmp(relop, "!=") == 0) strncpy(binop, "bne", buf_len-1);
    else if (strcmp(relop, "<") == 0) strncpy(binop, "blt", buf_len-1);
    else if (strcmp(relop, ">") == 0) strncpy(binop, "bgt", buf_len-1);
    else if (strcmp(relop, "<=") == 0) strncpy(binop, "ble", buf_len-1);
    else if (strcmp(relop, ">=") == 0) strncpy(binop, "bge", buf_len-1);
    else strncpy(binop, "beq", buf_len-1);
}

// 核心翻译函数（集成寄存器分配初始化/收尾）
void generate_mips_code(InterCodes head, FILE *out) {
    assert(out != NULL);
    if (head == NULL) return;

    // 初始化：调用B部分寄存器分配接口
    regalloc_init();  // 关键：初始化寄存器分配器
    stack_map = NULL;
    stack_map_size = 0;
    stack_offset = STACK_BASE_OFFSET;

    // 初始化MIPS生成（输出.data段和read/write函数）
    mips_init(out);

    // 遍历中间代码链表（修复边界判断，避免空指针）
    InterCodes next = head;
    while (next != NULL) {  // 先判断next是否为空，再取code
        InterCode curr = next->code;
        if (curr == NULL) {
            next = next->next;
            continue;
        }

        char op1_buf[32], op2_buf[32], res_buf[32], label_buf[32], binop_buf[8];
        switch (curr->kind) {
            case IR_LABEL: {
                get_operand_str(curr->u.one.x, label_buf, sizeof(label_buf), out);
                emit_label(out, label_buf);
                break;
            }
            case IR_FUNCTION: {
                // 函数入口：初始化栈帧 + 寄存器分配
                get_operand_str(curr->u.one.x, res_buf, sizeof(res_buf), out);
                emit_text_begin(out);
                fprintf(out, ".globl %s\n", res_buf);
                fprintf(out, "%s:\n", res_buf);
                // 栈帧初始化（保存$fp/$ra，设置栈基址）
                emit_addi(out, "$sp", "$sp", -stack_offset);  // 预留栈空间
                emit_sw(out, "$fp", 0, "$sp");
                emit_sw(out, "$ra", 4, "$sp");
                emit_move(out, "$fp", "$sp");
                break;
            }
            case IR_ASSIGN: {
                // 赋值：dst := src → 处理寄存器/栈存储
                get_operand_str(curr->u.assign.left, res_buf, sizeof(res_buf), out);
                get_operand_str(curr->u.assign.right, op1_buf, sizeof(op1_buf), out);
                
                char left_var[64] = {0};
                if (curr->u.assign.left->kind == OP_VARIABLE) {
                    strncpy(left_var, curr->u.assign.left->u.name, sizeof(left_var)-1);
                } else if (curr->u.assign.left->kind == OP_TEMP) {
                    snprintf(left_var, sizeof(left_var), "t%d", curr->u.assign.left->u.no);
                }

                // 若目标变量溢出到栈，需将结果写回栈
                int left_reg = get_reg(left_var);
                if (left_reg < 0 || left_reg >= REG_COUNT) {
                    int left_stack = find_stack_offset(left_var);
                    emit_sw(out, op1_buf, left_stack, "$fp");  // 写回栈
                } else {
                    if (curr->u.assign.right->kind == OP_CONSTANT) {
                        emit_li(out, res_buf, atoi(op1_buf));
                    } else {
                        emit_move(out, res_buf, op1_buf);
                    }
                }
                break;
            }
            case IR_ADD:
            case IR_SUB:
            case IR_MUL:
            case IR_DIV: {
                get_operand_str(curr->u.binop.result, res_buf, sizeof(res_buf), out);
                get_operand_str(curr->u.binop.op1, op1_buf, sizeof(op1_buf), out);
                get_operand_str(curr->u.binop.op2, op2_buf, sizeof(op2_buf), out);
                switch (curr->kind) {
                    case IR_ADD: emit_add(out, res_buf, op1_buf, op2_buf); break;
                    case IR_SUB: emit_sub(out, res_buf, op1_buf, op2_buf); break;
                    case IR_MUL: emit_mul(out, res_buf, op1_buf, op2_buf); break;
                    case IR_DIV: emit_div(out, res_buf, op1_buf, op2_buf); break;
                }
                // 结果写回栈（若目标变量溢出）
                char res_var[64] = {0};
                if (curr->u.binop.result->kind == OP_VARIABLE) {
                    strncpy(res_var, curr->u.binop.result->u.name, sizeof(res_var)-1);
                } else if (curr->u.binop.result->kind == OP_TEMP) {
                    snprintf(res_var, sizeof(res_var), "t%d", curr->u.binop.result->u.no);
                }
                int res_reg = get_reg(res_var);
                if (res_reg < 0 || res_reg >= REG_COUNT) {
                    int res_stack = find_stack_offset(res_var);
                    emit_sw(out, res_buf, res_stack, "$fp");
                }
                break;
            }
            case IR_GOTO: {
                get_operand_str(curr->u.one.x, label_buf, sizeof(label_buf), out);
                emit_j(out, label_buf);
                break;
            }
            case IR_IF_GOTO: {
                get_operand_str(curr->u.if_goto.x, op1_buf, sizeof(op1_buf), out);
                get_operand_str(curr->u.if_goto.y, op2_buf, sizeof(op2_buf), out);
                get_operand_str(curr->u.if_goto.label, label_buf, sizeof(label_buf), out);
                map_relop_to_binop(curr->u.if_goto.relop, binop_buf, sizeof(binop_buf));
                
                // 处理常量操作数（加载到临时寄存器）
                if (curr->u.if_goto.y->kind == OP_CONSTANT) {
                    emit_li(out, "$t9", atoi(op2_buf));
                    strncpy(op2_buf, "$t9", sizeof(op2_buf)-1);
                }
                if (curr->u.if_goto.x->kind == OP_CONSTANT) {
                    emit_li(out, "$t8", atoi(op1_buf));
                    strncpy(op1_buf, "$t8", sizeof(op1_buf)-1);
                }

                // 生成分支指令
                if (strcmp(binop_buf, "beq") == 0) emit_beq(out, op1_buf, op2_buf, label_buf);
                else if (strcmp(binop_buf, "bne") == 0) emit_bne(out, op1_buf, op2_buf, label_buf);
                else if (strcmp(binop_buf, "blt") == 0) emit_blt(out, op1_buf, op2_buf, label_buf);
                else if (strcmp(binop_buf, "bgt") == 0) emit_bgt(out, op1_buf, op2_buf, label_buf);
                else if (strcmp(binop_buf, "ble") == 0) emit_ble(out, op1_buf, op2_buf, label_buf);
                else if (strcmp(binop_buf, "bge") == 0) emit_bge(out, op1_buf, op2_buf, label_buf);
                break;
            }
            case IR_RETURN: {
                // 返回：处理返回值 + 恢复栈帧 + 收尾寄存器分配
                get_operand_str(curr->u.one.x, op1_buf, sizeof(op1_buf), out);
                emit_move(out, "$v0", op1_buf);  // 返回值存入$v0
                
                // 恢复栈帧
                emit_lw(out, "$ra", 4, "$fp");
                emit_lw(out, "$fp", 0, "$fp");
                emit_addi(out, "$sp", "$sp", stack_offset);
                
                // 调用B部分接口：收尾寄存器分配
                regalloc_finish();  // 关键：释放寄存器分配资源
                
                emit_jr(out, "$ra");
                break;
            }
            case IR_READ: {
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
            }
            case IR_WRITE: {
                get_operand_str(curr->u.one.x, op1_buf, sizeof(op1_buf), out);
                emit_move(out, "$a0", op1_buf);
                emit_jal(out, "write");
                break;
            }
            case IR_CALL: {
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
            }
            case IR_PARAM: {  // 修复：添加{}包裹static变量
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
            }
            case IR_ARG: {  // 修复：添加{}包裹static变量
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
            }
            case IR_LOAD: {
                get_operand_str(curr->u.load.left, res_buf, sizeof(res_buf), out);
                get_operand_str(curr->u.load.right, op1_buf, sizeof(op1_buf), out);
                emit_lw(out, res_buf, 0, op1_buf);
                break;
            }
            case IR_STORE: {
                get_operand_str(curr->u.store.addr, op1_buf, sizeof(op1_buf), out);
                get_operand_str(curr->u.store.value, op2_buf, sizeof(op2_buf), out);
                emit_sw(out, op2_buf, 0, op1_buf);
                break;
            }
            case IR_DEC: {  // 修复：添加{}包裹Operand声明
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
            }
            default: {
                fprintf(out, "    # Unhandled IR kind: %d\n", curr->kind);
                break;
            }
        }
        next = next->next;
    }

    // 程序退出指令
    emit_label(out, "exit");
    emit_li(out, "$v0", 10);
    emit_syscall(out);

    // 清理栈映射表
    if (stack_map != NULL) {
        free(stack_map);
        stack_map = NULL;
    }
}

// 对外统一接口（保持不变）
void generate_mips(InterCodes head, FILE *output_file) {
    generate_mips_code(head, output_file);
}

// 调试用：打印中间代码
void print_intercodes_for_debug(InterCodes head) {
    if (head == NULL) return;
    InterCodes curr = head;
    while (curr != NULL) {
        // print_intercode(stdout, curr->code);  // 注释保留，按需启用
        curr = curr->next;
    }
}