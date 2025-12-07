#include "ir.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void* xmalloc(size_t n) {
    void* p = malloc(n);
    if (!p) {
        fprintf(stderr, "Out of memory in IR module\n");
        exit(2);
    }
    return p;
}

static char* xstrdup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* p = (char*)xmalloc(n);
    memcpy(p, s, n);
    return p;
}

InterCodes ir_head = NULL;
InterCodes ir_tail = NULL;

static int temp_no = 1;
static int label_no = 1;

void ir_init(void) {
    ir_head = ir_tail = NULL;
    temp_no = 1;
    label_no = 1;
}

void ir_append(InterCode code) {
    if (!code) return;
    InterCodes node = (InterCodes)xmalloc(sizeof(*node));
    node->code = code;
    node->prev = ir_tail;
    node->next = NULL;
    if (!ir_head) ir_head = node;
    if (ir_tail) ir_tail->next = node;
    ir_tail = node;
}

void ir_free_all(void) {
    InterCodes p = ir_head;
    while (p) {
        InterCodes nxt = p->next;
        // 注意：这里简化释放；实际可递归释放 Operand 和字符串
        if (p->code->kind == IR_IF_GOTO) {
            free(p->code->u.if_goto.relop);
        }
        free(p->code);
        free(p);
        p = nxt;
    }
    ir_head = ir_tail = NULL;
}

/* ========== Operand 构造 ========== */
Operand op_variable(const char* name) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_VARIABLE;
    op->u.name = xstrdup(name);
    return op;
}

Operand op_temp(void) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_TEMP;
    op->u.no = temp_no++;
    return op;
}

Operand op_constant(int value) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_CONSTANT;
    op->u.value = value;
    return op;
}

Operand op_label(void) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_LABEL;
    op->u.no = label_no++;
    return op;
}

Operand op_function(const char* name) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_FUNCTION;
    op->u.name = xstrdup(name);
    return op;
}

Operand op_address(Operand base) {
    Operand op = (Operand)xmalloc(sizeof(*op));
    op->kind = OP_ADDRESS;
    op->u.base = base;
    return op;
}

/* ========== InterCode 构造 ========== */
static InterCode ir_new_with_kind(int kind) {
    InterCode c = (InterCode)xmalloc(sizeof(*c));
    c->kind = kind;
    return c;
}

InterCode ir_make_label(Operand label) {
    InterCode c = ir_new_with_kind(IR_LABEL);
    c->u.one.x = label;
    return c;
}

InterCode ir_make_function(Operand func) {
    InterCode c = ir_new_with_kind(IR_FUNCTION);
    c->u.one.x = func;
    return c;
}

InterCode ir_make_assign(Operand left, Operand right) {
    InterCode c = ir_new_with_kind(IR_ASSIGN);
    c->u.assign.left = left;
    c->u.assign.right = right;
    return c;
}

InterCode ir_make_binop(int opKind, Operand result, Operand op1, Operand op2) {
    InterCode c = ir_new_with_kind(opKind);
    c->u.binop.result = result;
    c->u.binop.op1 = op1;
    c->u.binop.op2 = op2;
    return c;
}

InterCode ir_make_goto(Operand label) {
    InterCode c = ir_new_with_kind(IR_GOTO);
    c->u.one.x = label;
    return c;
}

InterCode ir_make_if_goto(Operand x, const char* relop, Operand y, Operand label) {
    InterCode c = ir_new_with_kind(IR_IF_GOTO);
    c->u.if_goto.x = x;
    c->u.if_goto.y = y;
    c->u.if_goto.label = label;
    c->u.if_goto.relop = xstrdup(relop);
    return c;
}

InterCode ir_make_return(Operand x) {
    InterCode c = ir_new_with_kind(IR_RETURN);
    c->u.one.x = x;
    return c;
}

InterCode ir_make_dec(Operand x, int size) {
    InterCode c = ir_new_with_kind(IR_DEC);
    c->u.dec.x = x;
    c->u.dec.size = size;
    return c;
}

InterCode ir_make_arg(Operand x) {
    InterCode c = ir_new_with_kind(IR_ARG);
    c->u.one.x = x;
    return c;
}

InterCode ir_make_call(Operand result, Operand func) {
    InterCode c = ir_new_with_kind(IR_CALL);
    c->u.call.result = result;
    c->u.call.func = func;
    return c;
}

InterCode ir_make_param(Operand x) {
    InterCode c = ir_new_with_kind(IR_PARAM);
    c->u.one.x = x;
    return c;
}

InterCode ir_make_read(Operand x) {
    InterCode c = ir_new_with_kind(IR_READ);
    c->u.one.x = x;
    return c;
}

InterCode ir_make_write(Operand x) {
    InterCode c = ir_new_with_kind(IR_WRITE);
    c->u.one.x = x;
    return c;
}

// ========== 新增：LOAD / STORE ========== //
InterCode ir_make_load(Operand left, Operand addr) {
    InterCode c = ir_new_with_kind(IR_LOAD);
    c->u.load.left = left;
    c->u.load.right = addr;
    return c;
}

InterCode ir_make_store(Operand addr, Operand value) {
    InterCode c = ir_new_with_kind(IR_STORE);
    c->u.store.addr = addr;
    c->u.store.value = value;
    return c;
}

/* ========== 打印 ========== */
static void print_operand(FILE* out, Operand op) {
    if (!op) return;
    switch (op->kind) {
        case OP_VARIABLE: fprintf(out, "%s", op->u.name); break;
        case OP_TEMP: fprintf(out, "t%d", op->u.no); break;
        case OP_CONSTANT: fprintf(out, "#%d", op->u.value); break;
        case OP_LABEL: fprintf(out, "label%d", op->u.no); break;
        case OP_FUNCTION: fprintf(out, "%s", op->u.name); break;
        case OP_ADDRESS:
            fprintf(out, "&");
            print_operand(out, op->u.base);
            break;
    }
}

static void print_intercode(FILE* out, InterCode c) {
    switch (c->kind) {
        case IR_LABEL:
            fprintf(out, "LABEL "); print_operand(out, c->u.one.x); fprintf(out, " :\n");
            break;
        case IR_FUNCTION:
            fprintf(out, "FUNCTION "); print_operand(out, c->u.one.x); fprintf(out, " :\n");
            break;
        case IR_ASSIGN:
            print_operand(out, c->u.assign.left);
            fprintf(out, " := ");
            print_operand(out, c->u.assign.right);
            fprintf(out, "\n");
            break;
        case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV: {
            char op_char = (c->kind == IR_ADD ? '+' :
                           (c->kind == IR_SUB ? '-' :
                           (c->kind == IR_MUL ? '*' : '/')));
            print_operand(out, c->u.binop.result);
            fprintf(out, " := ");
            print_operand(out, c->u.binop.op1);
            fprintf(out, " %c ", op_char);
            print_operand(out, c->u.binop.op2);
            fprintf(out, "\n");
            break;
        }
        case IR_GOTO:
            fprintf(out, "GOTO "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_IF_GOTO:
            fprintf(out, "IF ");
            print_operand(out, c->u.if_goto.x);
            fprintf(out, " %s ", c->u.if_goto.relop);
            print_operand(out, c->u.if_goto.y);
            fprintf(out, " GOTO ");
            print_operand(out, c->u.if_goto.label);
            fprintf(out, "\n");
            break;
        case IR_RETURN:
            fprintf(out, "RETURN "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_DEC:
            fprintf(out, "DEC "); print_operand(out, c->u.dec.x);
            fprintf(out, " %d\n", c->u.dec.size);
            break;
        case IR_ARG:
            fprintf(out, "ARG "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_CALL:
            print_operand(out, c->u.call.result);
            fprintf(out, " := CALL ");
            print_operand(out, c->u.call.func);
            fprintf(out, "\n");
            break;
        case IR_PARAM:
            fprintf(out, "PARAM "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_READ:
            fprintf(out, "READ "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_WRITE:
            fprintf(out, "WRITE "); print_operand(out, c->u.one.x); fprintf(out, "\n");
            break;
        case IR_LOAD:
            print_operand(out, c->u.load.left);
            fprintf(out, " := *");
            print_operand(out, c->u.load.right);
            fprintf(out, "\n");
            break;
        case IR_STORE:
            fprintf(out, "*");
            print_operand(out, c->u.store.addr);
            fprintf(out, " := ");
            print_operand(out, c->u.store.value);
            fprintf(out, "\n");
            break;
    }
}

void ir_print_all(FILE* out) {
    for (InterCodes p = ir_head; p; p = p->next) {
        print_intercode(out, p->code);
    }
}


// ===================== 实验四新增实现 =====================
/**
 * 调试用打印中间代码（直接复用实验三的 print_intercodes 函数）
 */
void print_intercodes_for_debug(InterCode head) {
    print_intercodes(stdout, head);
}

/**
 * 转发到代码生成模块的核心函数
 */
void generate_mips(InterCode head, FILE *output_file) {
    // 调用 codegen.c 中的核心翻译函数
    generate_mips_code(head, output_file);
}