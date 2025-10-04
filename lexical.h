#ifndef LEXICAL_H
#define LEXICAL_H

#include "tree.h"  

// ���� ASTNodeList �ṹ��
typedef struct ASTNodeList {
    ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

// �����ⲿ����
extern int lexical_error_occurred;
extern ASTNodeList* ast_node_list;

// ��������
void add_ast_node(ASTNode* node);
void print_and_free_ast_nodes();
void free_ast_nodes();

#endif