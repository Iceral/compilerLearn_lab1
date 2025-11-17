#ifndef IR_H_
#define IR_H_

#include <stdio.h>

/* ========== Operand ==========
 * 代表三地址代码中的操作数：
 *  - 变量名（原始变量）
 *  - 临时变量 t1, t2, ...
 *  - 常量 #k
 *  - 标号 label1, label2, ...
 *  - 函数名 f
 *  - 地址 &x / *p 之类（若需要）
 */
typedef struct Operand_* Operand;
struct Operand_ {
    enum {
        OP_VARIABLE,   /* 变量：按名字区分 */
        OP_TEMP,       /* 临时变量 t1, t2... */
        OP_CONSTANT,   /* 常量 #k */
        OP_LABEL,      /* 标号 label1... */
        OP_FUNCTION,   /* 函数名 */
        OP_ADDRESS     /* 地址形式（可选，用于数组/结构体） */
    } kind;

    union {
        int   no;      /* 用于 TEMP / LABEL 的序号 */
        int   value;   /* CONSTANT 的整数值 */
        char* name;    /* VARIABLE / FUNCTION 的名字 */
        Operand base;  /* ADDRESS：指向“基准操作数”，可选 */
    } u;
};

/* ========== InterCode ==========
 * 一条中间代码：
 *   LABEL x :
 *   FUNCTION f :
 *   x := y
 *   x := y + z
 *   GOTO x
 *   IF x relop y GOTO z
 *   RETURN x
 *   DEC x size
 *   ARG x
 *   x := CALL f
 *   PARAM x
 *   READ x
 *   WRITE x
 */
typedef struct InterCode_* InterCode;
struct InterCode_ {
    enum {
        IR_LABEL,
        IR_FUNCTION,
        IR_ASSIGN,      /* x := y */
        IR_ADD,         /* x := y + z */
        IR_SUB,         /* x := y - z */
        IR_MUL,         /* x := y * z */
        IR_DIV,         /* x := y / z */
        IR_GOTO,
        IR_IF_GOTO,     /* IF x relop y GOTO z */
        IR_RETURN,
        IR_DEC,
        IR_ARG,
        IR_CALL,        /* x := CALL f */
        IR_PARAM,
        IR_READ,
        IR_WRITE
    } kind;

    union {
        struct { Operand x; } one;                         /* LABEL, FUNCTION, RETURN, ARG, PARAM, READ, WRITE, GOTO */
        struct { Operand left, right; } assign;            /* ASSIGN */
        struct { Operand result, op1, op2; } binop;        /* ADD/SUB/MUL/DIV */
        struct { Operand x, y, label; char* relop; } if_goto; /* IF x relop y GOTO label */
        struct { Operand x; int size; } dec;               /* DEC x size */
        struct { Operand result, func; } call;             /* result := CALL func */
    } u;
};

/* 双向链表，用来保存所有 IR */
typedef struct InterCodes_* InterCodes;
struct InterCodes_ {
    InterCode code;
    InterCodes prev, next;
};

/* 全局 IR 链表头尾（由 ir_init 初始化） */
extern InterCodes ir_head;
extern InterCodes ir_tail;

/* ========== 对外接口：由你实现，供翻译模块调用 ========== */

/* 初始化/销毁 */
void ir_init(void);
void ir_free_all(void);

/* 追加一条指令到链表末尾 */
void ir_append(InterCode code);

/* -------- Operand 构造函数 -------- */
Operand op_variable(const char* name);
Operand op_temp(void);          /* 自动分配 t1, t2, ... */
Operand op_constant(int value); /* #value */
Operand op_label(void);         /* 自动分配 label1, label2... */
Operand op_function(const char* name);
Operand op_address(Operand base); /* 可选：用于 &x, *p 等，后续 B/C 视需要使用 */

/* -------- InterCode 构造函数 -------- */
InterCode ir_make_label(Operand label);
InterCode ir_make_function(Operand func);
InterCode ir_make_assign(Operand left, Operand right);
InterCode ir_make_binop(int opKind, Operand result, Operand op1, Operand op2);
/* opKind 取 IR_ADD / IR_SUB / IR_MUL / IR_DIV 之一 */

InterCode ir_make_goto(Operand label);
InterCode ir_make_if_goto(Operand x, const char* relop, Operand y, Operand label);
InterCode ir_make_return(Operand x);
InterCode ir_make_dec(Operand x, int size);
InterCode ir_make_arg(Operand x);
InterCode ir_make_call(Operand result, Operand func);
InterCode ir_make_param(Operand x);
InterCode ir_make_read(Operand x);
InterCode ir_make_write(Operand x);

/* 把整个 IR 链表打印到文件（实验三要用） */
void ir_print_all(FILE* out);

#endif /* IR_H_ */
