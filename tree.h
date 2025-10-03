#ifndef AST_TREE_H
#define AST_TREE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* =============== 结点类型 =============== */
typedef enum {
    ASTK_NONTERM = 0,  /* 非终结符 */
    ASTK_ID,           /* 词法：ID（打印 ID: name） */
    ASTK_TYPE,         /* 词法：TYPE（打印 TYPE: int/float） */
    ASTK_INT,          /* 词法：INT（打印 INT: <十进制值>） */
    ASTK_FLOAT,        /* 词法：FLOAT（打印 FLOAT: <十进制值>） */
    ASTK_TOKEN         /* 其它终结符（如 PLUS/LP/SEMI 等，默认只打印名称） */
} ASTKind;

/* =============== AST 结点结构 =============== */
typedef struct ASTNode {
    const char* name;     /* 节点名：非终结符/终结符名称（如 "Exp" / "INT"） */
    int line;             /* 行号：非空非终结符打印用；词法叶子可置 0 */
    ASTKind kind;         /* 区分打印附加信息的方式 */

    /* 词法附加信息：只在相应 kind 下有效 */
    char* sval;           /* 对于 ID/TYPE：保存词素或具体类型名 */
    long long ival;       /* 对于 INT：十进制值（已转换） */
    double fval;          /* 对于 FLOAT：十进制值（已转换） */

    /* 子结点 */
    int nchild;
    struct ASTNode** child;
} ASTNode;

/* =============== 构造函数（供 Bison/Flex 调用） =============== */

/* 非终结符：传入名字、行号、子结点个数以及可变参数（子结点指针） */
ASTNode* ast_new_nonterm(const char* name, int line, int nchild, ...);

/* 终结符叶子（词法） */
ASTNode* ast_new_id   (const char* lexeme);     /* ID -> "ID: <lexeme>"   */
ASTNode* ast_new_type (const char* tyname);     /* TYPE -> "TYPE: int/float" */
ASTNode* ast_new_int  (long long v);            /* INT  -> "INT: <v>"     */
ASTNode* ast_new_float(double v);               /* FLOAT-> "FLOAT: <v>"   */

/* 其它终结符（如 PLUS/SEMI/LP 等），仅打印名称，不带附加值 */
ASTNode* ast_new_token(const char* name);

/* =============== 实用辅助 =============== */

/* 设置节点行号（常用于 “取首个词素的行号” 的规约动作后补） */
void ast_set_line(ASTNode* node, int line);

/* 从第一个非空子结点继承行号的便捷函数（无子结点则用 fallback） */
int  ast_first_line_fallback(int fallback, int nchild, ...);

/* 打印与释放 */
void ast_print(const ASTNode* node, int indent); /* 先序、每层缩进 2 空格；遵循实验输出规则 */
void ast_free (ASTNode* node);

#ifdef __cplusplus
}
#endif

#endif /* AST_TREE_H */
