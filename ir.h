#ifndef IR_H_
#define IR_H_

#include <stdio.h>

/* ========== Operand ========== */
typedef struct Operand_* Operand;
struct Operand_ {
    enum {
        OP_VARIABLE,
        OP_TEMP,
        OP_CONSTANT,
        OP_LABEL,
        OP_FUNCTION,
        OP_ADDRESS
    } kind;
    union {
        int no;         /* TEMP / LABEL */
        int value;      /* CONSTANT */
        char* name;     /* VARIABLE / FUNCTION */
        Operand base;   /* ADDRESS */
    } u;
};

/* ========== InterCode ========== */
typedef struct InterCode_* InterCode;
struct InterCode_ {
    enum {
        IR_LABEL,
        IR_FUNCTION,
        IR_ASSIGN,
        IR_ADD,      // ← 算术运算：t := t1 + t2
        IR_SUB,      // ← t := t1 - t2
        IR_MUL,      // ← t := t1 * t2
        IR_DIV,      // ← t := t1 / t2
        IR_GOTO,
        IR_IF_GOTO,
        IR_RETURN,
        IR_DEC,
        IR_ARG,
        IR_CALL,
        IR_PARAM,
        IR_READ,
        IR_WRITE,
        IR_LOAD,     // ← x := *y
        IR_STORE     // ← *x := y
    } kind;
    union {
        struct { Operand x; } one; /* LABEL, FUNCTION, RETURN, PARAM, READ, WRITE, GOTO */
        struct { Operand left, right; } assign; /* ASSIGN */
        struct { Operand result, op1, op2; } binop; /* ADD/SUB/MUL/DIV */
        struct { Operand x, y, label; char* relop; } if_goto;
        struct { Operand x; int size; } dec;
        struct { Operand result, func; } call;
        struct { Operand left, right; } load;   // left := *right
        struct { Operand addr, value; } store;  // *addr := value
    } u;
};

/* 双向链表 */
typedef struct InterCodes_* InterCodes;
struct InterCodes_ {
    InterCode code;
    InterCodes prev, next;
};

extern InterCodes ir_head;
extern InterCodes ir_tail;

/* 初始化/销毁 */
void ir_init(void);
void ir_free_all(void);
void ir_append(InterCode code);

/* Operand 构造 */
Operand op_variable(const char* name);
Operand op_temp(void);
Operand op_constant(int value);
Operand op_label(void);
Operand op_function(const char* name);
Operand op_address(Operand base);

/* InterCode 构造 */
InterCode ir_make_label(Operand label);
InterCode ir_make_function(Operand func);
InterCode ir_make_assign(Operand left, Operand right);
InterCode ir_make_binop(int opKind, Operand result, Operand op1, Operand op2);
InterCode ir_make_goto(Operand label);
InterCode ir_make_if_goto(Operand x, const char* relop, Operand y, Operand label);
InterCode ir_make_return(Operand x);
InterCode ir_make_dec(Operand x, int size);
InterCode ir_make_arg(Operand x);
InterCode ir_make_call(Operand result, Operand func);
InterCode ir_make_param(Operand x);
InterCode ir_make_read(Operand x);
InterCode ir_make_write(Operand x);
InterCode ir_make_load(Operand left, Operand addr);
InterCode ir_make_store(Operand addr, Operand value);

/* 打印 */
void ir_print_all(FILE* out);

// ===================== 实验四新增内容（核心修改点） =====================
/**
 * @brief 调试用：打印中间代码（供代码生成模块调用）
 * @param head 中间代码链表头节点
 */
void print_intercodes_for_debug(InterCodes head);

/**
 * @brief 对外统一接口：将中间代码翻译为MIPS指令
 * @param head 中间代码链表头节点
 * @param output_file MIPS指令输出文件指针
 */
void generate_mips(InterCodes head, FILE *output_file);

#endif /* IR_H_ */