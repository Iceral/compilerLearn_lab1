#include "codegen.h"
#include "mips.h"
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================== 寄存器分配器 ==============================
#define REG_COUNT 8 
static const char *reg_names[REG_COUNT] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7"
};

static int reg_used[REG_COUNT] = {0};
static int stack_offset = 0; 
static char current_func[64] = {0};

typedef struct {
    char name[64];
    int offset;
} VarInfo;
static VarInfo *var_table = NULL;
static int var_count = 0;

static void regalloc_reset(void) {
    memset(reg_used, 0, sizeof(reg_used));
    stack_offset = 0; 
    var_count = 0;
    if (var_table) free(var_table);
    var_table = NULL;
    // 【修复1】不要在这里清空 current_func，否则函数名会丢失
    // current_func[0] = '\0'; 
}

// 查找变量偏移，如果没有则分配
static int get_var_offset(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0)
            return var_table[i].offset;
    }
    var_table = realloc(var_table, (var_count + 1) * sizeof(VarInfo));
    assert(var_table);
    strncpy(var_table[var_count].name, name, 63);
    
    if (stack_offset == 0) stack_offset = 8; // 预留 fp, ra
    
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

static const char* temp_reg(int index) {
    return (index == 0) ? "$t8" : "$t9";
}

static int is_immediate(const char *str, int *value) {
    if (str[0] == '#') {
        *value = atoi(str + 1);
        return 1;
    }
    return 0;
}

static int is_memory(const char *op_str) {
    return strstr(op_str, "($fp)") != NULL;
}

static int parse_offset(const char *op_str) {
    return atoi(op_str); 
}

// 核心函数：获取操作数字符串
static void get_operand(Operand op, char *buf, int is_dest, FILE *out) {
    char name[64] = {0};

    if (!op) { strcpy(buf, "$zero"); return; }

    switch (op->kind) {
        case OP_CONSTANT:
            sprintf(buf, "#%d", op->u.value);
            return;
        
        // 变量：直接返回栈地址
        case OP_VARIABLE:
            {
                int offset = get_var_offset(op->u.name);
                sprintf(buf, "-%d($fp)", offset);
                return;
            }

        // 【修复2】临时变量（t1, t2...）：也强制分配栈空间
        // 这样可以解决 "add $t2" 下一句变成 "sw $t3" 导致数据断片的问题
        case OP_TEMP:
            {
                sprintf(name, "t%d", op->u.no);
                int offset = get_var_offset(name);
                sprintf(buf, "-%d($fp)", offset);
                return;
            }
            
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

// ============================== 预扫描 ==============================
int compute_frame_size(InterCodes start) {
    InterCodes p = start->next; 
    
    while (p && p->code->kind != IR_FUNCTION) {
        InterCode c = p->code;
        // 简单粗暴：扫描所有可能产生变量的操作数，提前分配 offset
        Operand ops[] = { 
            c->u.assign.left, c->u.assign.right,
            c->u.binop.result, c->u.binop.op1, c->u.binop.op2,
            c->u.one.x, c->u.dec.x 
        };
        
        // 针对不同指令类型，手动解包 Operand
        // 只要调用 get_operand 内部逻辑里用到的 get_var_offset 即可
        // 这里为了简单，我们模拟一遍 get_var_offset 调用
        
        if (c->kind == IR_ASSIGN || c->kind == IR_LOAD || c->kind == IR_STORE) {
            if(c->u.assign.left && (c->u.assign.left->kind==OP_VARIABLE || c->u.assign.left->kind==OP_TEMP)) {
                char n[64];
                if(c->u.assign.left->kind==OP_TEMP) sprintf(n, "t%d", c->u.assign.left->u.no);
                else strcpy(n, c->u.assign.left->u.name);
                get_var_offset(n);
            }
            if(c->u.assign.right && (c->u.assign.right->kind==OP_VARIABLE || c->u.assign.right->kind==OP_TEMP)) {
                char n[64];
                if(c->u.assign.right->kind==OP_TEMP) sprintf(n, "t%d", c->u.assign.right->u.no);
                else strcpy(n, c->u.assign.right->u.name);
                get_var_offset(n);
            }
        }
        else if (c->kind >= IR_ADD && c->kind <= IR_DIV) {
            Operand os[] = {c->u.binop.result, c->u.binop.op1, c->u.binop.op2};
            for(int i=0; i<3; i++) {
                if(os[i] && (os[i]->kind==OP_VARIABLE || os[i]->kind==OP_TEMP)) {
                    char n[64];
                    if(os[i]->kind==OP_TEMP) sprintf(n, "t%d", os[i]->u.no);
                    else strcpy(n, os[i]->u.name);
                    get_var_offset(n);
                }
            }
        }
        else if (c->kind == IR_READ || c->kind == IR_WRITE || c->kind == IR_CALL || c->kind == IR_ARG || c->kind == IR_RETURN || c->kind == IR_CALL) {
             Operand x = c->u.one.x;
             if (c->kind == IR_CALL) x = c->u.call.result;
             if(x && (x->kind==OP_VARIABLE || x->kind==OP_TEMP)) {
                char n[64];
                if(x->kind==OP_TEMP) sprintf(n, "t%d", x->u.no);
                else strcpy(n, x->u.name);
                get_var_offset(n);
             }
        }
        else if (c->kind == IR_DEC) {
            char base[64];
            if (c->u.dec.x->kind == OP_VARIABLE) strcpy(base, c->u.dec.x->u.name);
            else sprintf(base, "t%d", c->u.dec.x->u.no);
            for (int i = 0; i < c->u.dec.size; i += 4) {
                char name[70];
                sprintf(name, "%s_%d", base, i);
                get_var_offset(name);
            }
        }
        
        p = p->next;
    }
    return stack_offset;
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
        int imm_val;

        switch (c->kind) {
            case IR_FUNCTION: {
                // 1. 先复制函数名
                char func_name[64];
                strcpy(func_name, c->u.one.x->u.name);
                
                // 2. 重置环境 (现在不会清空 func_name 了)
                regalloc_reset();
                strcpy(current_func, func_name); // 再设置一遍全局变量
                
                // 3. 预扫描
                InterCodes backup_p = p;
                frame_size = compute_frame_size(p); 
                p = backup_p;
                
                // 4. 打印函数头
                fprintf(out, "\n.globl %s\n", current_func);
                emit_label(out, current_func);

                // 5. Prologue
                if (frame_size > 0) {
                    emit_addi(out, "$sp", "$sp", -frame_size);
                    emit_sw(out, "$ra", frame_size - 4, "$sp");
                    emit_sw(out, "$fp", frame_size - 8, "$sp");
                    emit_addi(out, "$fp", "$sp", frame_size - 8);
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
                get_operand(c->u.assign.right, x, 0, out); 
                get_operand(c->u.assign.left,  z, 1, out); 

                // 统一处理：不管是变量还是temp，现在都是内存地址 "-8($fp)"
                // 或者立即数 "#1"
                
                if (is_immediate(x, &imm_val)) {
                    if (is_memory(z)) {
                        emit_li(out, temp_reg(0), imm_val);
                        emit_sw(out, temp_reg(0), parse_offset(z), "$fp");
                    } else {
                        emit_li(out, z, imm_val); // 不太可能到这，除非z是寄存器
                    }
                } else {
                    // 内存到内存的拷贝
                    if (is_memory(x)) {
                        emit_lw(out, temp_reg(0), parse_offset(x), "$fp");
                        if (is_memory(z)) {
                            emit_sw(out, temp_reg(0), parse_offset(z), "$fp");
                        }
                    }
                }
                break;
            }
            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
                get_operand(c->u.binop.op1, x, 0, out);
                get_operand(c->u.binop.op2, y, 0, out);
                get_operand(c->u.binop.result, z, 1, out);

                // 全栈式操作：lw -> alu -> sw
                // x 和 y 都在栈里，加载到 $t8, $t9
                char *rx = (char*)temp_reg(0);
                char *ry = (char*)temp_reg(1);
                
                // Op1
                if (is_immediate(x, &imm_val)) emit_li(out, rx, imm_val);
                else emit_lw(out, rx, parse_offset(x), "$fp");
                
                // Op2
                if (is_immediate(y, &imm_val)) emit_li(out, ry, imm_val);
                else emit_lw(out, ry, parse_offset(y), "$fp");
                
                // Calc
                if (c->kind == IR_ADD) emit_add(out, rx, rx, ry);
                if (c->kind == IR_SUB) emit_sub(out, rx, rx, ry);
                if (c->kind == IR_MUL) emit_mul(out, rx, rx, ry);
                if (c->kind == IR_DIV) emit_div(out, rx, rx, ry);
                
                // Result
                if (is_memory(z)) {
                    emit_sw(out, rx, parse_offset(z), "$fp");
                }
                break;
            }

            case IR_CALL: {
                get_operand(c->u.call.result, z, 1, out);
                emit_jal(out, c->u.call.func->u.name);
                if (is_memory(z)) {
                    emit_sw(out, "$v0", parse_offset(z), "$fp");
                }
                break;
            }

            case IR_RETURN: {
                get_operand(c->u.one.x, x, 0, out);
                if (is_immediate(x, &imm_val)) {
                    emit_li(out, "$v0", imm_val);
                } else if (is_memory(x)) {
                    emit_lw(out, "$v0", parse_offset(x), "$fp");
                }

                if (frame_size > 0) {
                    emit_lw(out, "$ra", frame_size - 4, "$sp");
                    emit_lw(out, "$fp", frame_size - 8, "$sp");
                    emit_addi(out, "$sp", "$sp", frame_size);
                }
                emit_jr(out, "$ra");
                break;
            }

            case IR_ARG: {
                static int arg_idx = 0;
                if (p->prev && p->prev->code->kind != IR_ARG) arg_idx = 0;
                
                get_operand(c->u.one.x, x, 0, out);
                
                // 加载参数值到 $t8
                if (is_memory(x)) emit_lw(out, temp_reg(0), parse_offset(x), "$fp");
                else if (is_immediate(x, &imm_val)) emit_li(out, temp_reg(0), imm_val);

                if (arg_idx < 4) {
                    char a[8]; sprintf(a, "$a%d", arg_idx);
                    emit_move(out, a, temp_reg(0));
                } else {
                    emit_addi(out, "$sp", "$sp", -4);
                    emit_sw(out, temp_reg(0), 0, "$sp");
                }
                arg_idx++;
                break;
            }

            case IR_PARAM:
                {
                    static int param_idx = 0;
                    if (p->prev && p->prev->code->kind != IR_PARAM && p->prev->code->kind != IR_FUNCTION) param_idx = 0;
                    int offset = get_var_offset(c->u.one.x->u.name);
                    if (param_idx < 4) {
                        fprintf(out, "    sw $a%d, -%d($fp)\n", param_idx, offset);
                    }
                    param_idx++;
                }
                break;

            case IR_READ:
                get_operand(c->u.one.x, z, 1, out);
                emit_addi(out, "$sp", "$sp", -4); 
                emit_sw(out, "$ra", 0, "$sp");
                emit_jal(out, "read");
                emit_lw(out, "$ra", 0, "$sp");
                emit_addi(out, "$sp", "$sp", 4);
                if (is_memory(z)) emit_sw(out, "$v0", parse_offset(z), "$fp");
                break;

            case IR_WRITE:
                get_operand(c->u.one.x, x, 0, out);
                if (is_immediate(x, &imm_val)) emit_li(out, "$a0", imm_val);
                else if (is_memory(x)) emit_lw(out, "$a0", parse_offset(x), "$fp");
                
                emit_addi(out, "$sp", "$sp", -4);
                emit_sw(out, "$ra", 0, "$sp");
                emit_jal(out, "write");
                emit_lw(out, "$ra", 0, "$sp");
                emit_addi(out, "$sp", "$sp", 4);
                break;

            case IR_IF_GOTO: {
                get_operand(c->u.if_goto.x, x, 0, out);
                get_operand(c->u.if_goto.y, y, 0, out);
                sprintf(label, "label%d", c->u.if_goto.label->u.no);
                
                char *rx = (char*)temp_reg(0);
                char *ry = (char*)temp_reg(1);

                if (is_immediate(x, &imm_val)) emit_li(out, rx, imm_val);
                else emit_lw(out, rx, parse_offset(x), "$fp");
                
                if (is_immediate(y, &imm_val)) emit_li(out, ry, imm_val);
                else emit_lw(out, ry, parse_offset(y), "$fp");

                const char *rel = c->u.if_goto.relop;
                if (strcmp(rel, "==") == 0) emit_beq(out, rx, ry, label);
                else if (strcmp(rel, "!=") == 0) emit_bne(out, rx, ry, label);
                else if (strcmp(rel, "<")  == 0) emit_blt(out, rx, ry, label);
                else if (strcmp(rel, ">")  == 0) emit_bgt(out, rx, ry, label);
                else if (strcmp(rel, "<=") == 0) emit_ble(out, rx, ry, label);
                else if (strcmp(rel, ">=") == 0) emit_bge(out, rx, ry, label);
                break;
            }
            
            case IR_DEC: break;
            case IR_LOAD: { 
                get_operand(c->u.load.left, z, 1, out); 
                get_operand(c->u.load.right, x, 0, out); 
                // load z, *x (x is addr)
                // 1. Load x's value (the address) into $t8
                if (is_memory(x)) emit_lw(out, temp_reg(0), parse_offset(x), "$fp");
                // 2. Load value at address $t8 into $t9
                emit_lw(out, temp_reg(1), 0, temp_reg(0));
                // 3. Store $t9 into z
                if (is_memory(z)) emit_sw(out, temp_reg(1), parse_offset(z), "$fp");
                break; 
            }
            case IR_STORE: { 
                get_operand(c->u.store.addr, x, 0, out);
                get_operand(c->u.store.value, y, 0, out);
                // *x = y
                // 1. Load x (addr) into $t8
                if (is_memory(x)) emit_lw(out, temp_reg(0), parse_offset(x), "$fp");
                // 2. Load y (val) into $t9
                if (is_immediate(y, &imm_val)) emit_li(out, temp_reg(1), imm_val);
                else emit_lw(out, temp_reg(1), parse_offset(y), "$fp");
                // 3. Store $t9 to 0($t8)
                emit_sw(out, temp_reg(1), 0, temp_reg(0));
                break;
            }
            default: break;
        }
        p = p->next;
    }
    if (var_table) free(var_table);
}

void generate_mips(InterCodes head, FILE *out) {
    generate_mips_code(head, out);
}