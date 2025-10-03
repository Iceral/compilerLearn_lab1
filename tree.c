#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


static void* xmalloc(size_t n) {
    void* p = malloc(n);
    if (!p) { fprintf(stderr, "Out of memory\n"); exit(2); }
    return p;
}

static char* xstrdup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* p = (char*)xmalloc(n);
    memcpy(p, s, n);
    return p;
}


ASTNode* ast_new_nonterm(const char* name, int line, int nchild, ...) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name   = name;
    nd->line   = line;
    nd->kind   = ASTK_NONTERM;
    nd->sval   = NULL;
    nd->ival   = 0;
    nd->fval   = 0.0;
    nd->nchild = nchild;
    nd->child  = NULL;

    if (nchild > 0) {
        nd->child = (ASTNode**)xmalloc(sizeof(ASTNode*) * nchild);
        va_list ap; va_start(ap, nchild);
        for (int i = 0; i < nchild; ++i) {
            nd->child[i] = va_arg(ap, ASTNode*);
        }
        va_end(ap);
    }
    return nd;
}

ASTNode* ast_new_id(const char* lexeme) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name = "ID";
    nd->line = 0;
    nd->kind = ASTK_ID;
    nd->sval = xstrdup(lexeme);
    nd->ival = 0;
    nd->fval = 0.0;
    nd->nchild = 0;
    nd->child = NULL;
    return nd;
}

ASTNode* ast_new_type(const char* tyname) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name = "TYPE";
    nd->line = 0;
    nd->kind = ASTK_TYPE;
    nd->sval = xstrdup(tyname);
    nd->ival = 0;
    nd->fval = 0.0;
    nd->nchild = 0;
    nd->child = NULL;
    return nd;
}

ASTNode* ast_new_int(long long v) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name = "INT";
    nd->line = 0;
    nd->kind = ASTK_INT;
    nd->sval = NULL;
    nd->ival = v;
    nd->fval = 0.0;
    nd->nchild = 0;
    nd->child = NULL;
    return nd;
}

ASTNode* ast_new_float(double v) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name = "FLOAT";
    nd->line = 0;
    nd->kind = ASTK_FLOAT;
    nd->sval = NULL;
    nd->ival = 0;
    nd->fval = v;
    nd->nchild = 0;
    nd->child = NULL;
    return nd;
}

ASTNode* ast_new_token(const char* name) {
    ASTNode* nd = (ASTNode*)xmalloc(sizeof(ASTNode));
    nd->name = name;   /* e.g. "LP", "SEMI", "PLUS" */
    nd->line = 0;
    nd->kind = ASTK_TOKEN;
    nd->sval = NULL;
    nd->ival = 0;
    nd->fval = 0.0;
    nd->nchild = 0;
    nd->child = NULL;
    return nd;
}


void ast_set_line(ASTNode* node, int line) {
    if (node) node->line = line;
}

int ast_first_line_fallback(int fallback, int nchild, ...) {
    va_list ap; va_start(ap, nchild);
    for (int i = 0; i < nchild; ++i) {
        ASTNode* ch = va_arg(ap, ASTNode*);
        if (!ch) continue;
        /* 取“首个词素”的行号：优先非空非终结符的 line，
           若子节点是叶子（词法）则通常没有行号，由 Bison 外部决定 */
        if (ch->line > 0) { va_end(ap); return ch->line; }
        /* 也可以选择递归向下寻找，但通常 Bison 动作会直接传入 @x.first_line */
    }
    va_end(ap);
    return fallback;
}



static void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) putchar(' '), putchar(' ');
}

static void print_leaf(const ASTNode* nd, int indent) {
    print_indent(indent);
    switch (nd->kind) {
        case ASTK_ID:
            printf("%s: %s\n", nd->name, nd->sval ? nd->sval : "");
            break;
        case ASTK_TYPE:
            printf("%s: %s\n", nd->name, nd->sval ? nd->sval : "");
            break;
        case ASTK_INT:
            printf("%s: %lld\n", nd->name, nd->ival);
            break;
        case ASTK_FLOAT:
            printf("%s: %f\n", nd->name, nd->fval);
            break;
        case ASTK_TOKEN:
        default:
            printf("%s\n", nd->name);
            break;
    }
}

void ast_print(const ASTNode* nd, int indent) {
    if (!nd) return;

    if (nd->kind == ASTK_NONTERM) {
        /* ε 产生式：不打印（实验要求：产生 ε 的语法单元无需打印） */
        if (nd->nchild == 0) return;

        print_indent(indent);
        if (nd->line > 0) printf("%s (%d)\n", nd->name, nd->line);
        else              printf("%s\n", nd->name);

        for (int i = 0; i < nd->nchild; ++i)
            ast_print(nd->child[i], indent + 1);
    } else {
        /* 终结符（词法叶子或其它 token） */
        print_leaf(nd, indent);
    }
}



void ast_free(ASTNode* nd) {
    if (!nd) return;
    for (int i = 0; i < nd->nchild; ++i) ast_free(nd->child[i]);
    if (nd->sval) free(nd->sval);
    if (nd->child) free(nd->child);
    free(nd);
}
