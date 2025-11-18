// translate_ext.h
#ifndef TRANSLATE_EXT_H
#define TRANSLATE_EXT_H

#include "tree.h"
#include "ir.h"  // 因为函数返回 Operand / 使用 InterCode

// ------------------ 对外主接口 ------------------
void translate_ast(const ASTNode* root);

// ------------------ 内部翻译函数（按你要求全部暴露） ------------------
void translate_func_def(const ASTNode* func_def_node);
void translate_CompSt(const ASTNode* compst);
void translate_Stmt(const ASTNode* stmt);
void translate_Cond(const ASTNode* node, Operand label_true, Operand label_false);
void translate_StmtList(const ASTNode* stmt_list);
Operand translate_Exp(const ASTNode* exp);

// ------------------ 辅助函数 ------------------
int get_struct_field_offset(const char* field_name);

#endif