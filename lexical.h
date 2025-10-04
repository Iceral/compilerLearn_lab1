#ifndef LEXICAL_H
#define LEXICAL_H

#include "tree.h"  

// 定义 ASTNodeList 结构体
typedef struct ASTNodeList {
    ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

// 声明外部变量
extern int lexical_error_occurred;
extern ASTNodeList* ast_node_list;

// 声明函数
void add_ast_node(ASTNode* node);
void print_and_free_ast_nodes();
void free_ast_nodes();

#endif