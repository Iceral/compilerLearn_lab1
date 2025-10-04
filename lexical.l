%option yylineno
%option noyywrap
%option nounput
%option noinput

%{
#include "lexical.h" 
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "syntax.tab.h"

/*词法单元定义（后续与Bison结合时可替换为syntax.tab.h）*/
#define INT 1
#define FLOAT 2
#define ID 3
#define SEMI 4
#define COMMA 5
#define ASSIGNOP 6
#define RELOP 7
#define PLUS 8
#define MINUS 9
#define STAR 10
#define DIV 11
#define AND 12
#define OR 13
#define DOT 14
#define NOT 15
#define TYPE 16
#define LP 17
#define RP 18
#define LB 19
#define RB 20
#define LC 21
#define RC 22
#define STRUCT 23
#define RETURN 24
#define IF 25
#define ELSE 26
#define WHILE 27

/* 定义yylval用于传递AST节点 */

/* 错误标志 */
int lexical_error_occurred = 0;
ASTNodeList* ast_node_list = NULL; 

/*全局AST根节点（可选，用于打印）*/

ASTNode* ast_root = NULL;
%}

/* 正则别名 */
digit           [0-9]
letter          [a-zA-Z_]

/* 整数正则 */
decimal         ([1-9]{digit}*|0)
octal           0[0-7]+
hex             0[xX][0-9a-fA-F]+
illegal_octal   0[0-7]*[89]
illegal_hex     0[xX]([0-9a-fA-F]*[g-zG-Z]|[xX]*)

/* 浮点数正则 */
float_common    ([0-9]+\.[0-9]+|[0-9]*\.[0-9]+|[0-9]+\.)
exp_part        [eE][+-]?{digit}+
float_const     {float_common}{exp_part}?
illegal_float   {float_common}[eE][+-]?[a-zA-Z_]*

/* 标识符与注释 */
id              {letter}({letter}|{digit}){0,31}
single_comment  "//".*
multi_comment   "/\*"([^*]|\*+[^*/])*\*+"/"
unclosed_multi  "/\*"([^*]|\*+[^*/])*
whitespace      [ \t\r\n]

%%

{single_comment}    { /* 丢弃单行注释 */ }
{multi_comment}     { /* 丢弃多行注释 */ }
{unclosed_multi}    { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Unclosed multi-line comment\n", yylineno); 
}

{whitespace}        { /* 丢弃空白符 */ }

"int"               { yylval.node = ast_new_type("int"); add_ast_node(yylval.node);return TYPE; }
"float"             { yylval.node = ast_new_type("float"); add_ast_node(yylval.node);return TYPE; }
"struct"            { yylval.node = ast_new_token("STRUCT"); add_ast_node(yylval.node);return STRUCT; }
"return"            { yylval.node = ast_new_token("RETURN"); add_ast_node(yylval.node);return RETURN; }
"if"                { yylval.node = ast_new_token("IF"); add_ast_node(yylval.node);return IF; }
"else"              { yylval.node = ast_new_token("ELSE"); add_ast_node(yylval.node);return ELSE; }
"while"             { yylval.node = ast_new_token("WHILE"); add_ast_node(yylval.node);return WHILE; }

{id}                { yylval.node = ast_new_id(yytext); return ID; }

{illegal_octal}     { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal octal number '%s'\n", yylineno, yytext); 
}
{illegal_hex}       { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal hexadecimal number '%s'\n", yylineno, yytext); 
}
{hex}               { yylval.node = ast_new_int(strtoll(yytext, NULL, 16));add_ast_node(yylval.node); return INT; }
{octal}             { yylval.node = ast_new_int(strtoll(yytext, NULL, 8)); add_ast_node(yylval.node);return INT; }
{decimal}           { yylval.node = ast_new_int(strtoll(yytext, NULL, 10)); add_ast_node(yylval.node);return INT; }

{illegal_float}     { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal floating point number '%s'\n", yylineno, yytext); 
}
{float_const}       { yylval.node = ast_new_float(atof(yytext)); add_ast_node(yylval.node);return FLOAT; }

";"                 { yylval.node = ast_new_token("SEMI");add_ast_node(yylval.node); return SEMI; }
","                 { yylval.node = ast_new_token("COMMA");add_ast_node(yylval.node); return COMMA; }
"="                 { yylval.node = ast_new_token("ASSIGNOP");add_ast_node(yylval.node); return ASSIGNOP; }
">="                { yylval.node = ast_new_token("RELOP");add_ast_node(yylval.node); return RELOP; }
"<="                { yylval.node = ast_new_token("RELOP");add_ast_node(yylval.node); return RELOP; }
"=="                { yylval.node = ast_new_token("RELOP"); add_ast_node(yylval.node);return RELOP; }
"!="                { yylval.node = ast_new_token("RELOP");add_ast_node(yylval.node); return RELOP; }
">"                 { yylval.node = ast_new_token("RELOP"); add_ast_node(yylval.node);return RELOP; }
"<"                 { yylval.node = ast_new_token("RELOP");add_ast_node(yylval.node); return RELOP; }
"+"                 { yylval.node = ast_new_token("PLUS"); add_ast_node(yylval.node);return PLUS; }
"-"                 { yylval.node = ast_new_token("MINUS");add_ast_node(yylval.node); return MINUS; }
"*"                 { yylval.node = ast_new_token("STAR");add_ast_node(yylval.node); return STAR; }
"/"                 { yylval.node = ast_new_token("DIV");add_ast_node(yylval.node); return DIV; }
"&&"                { yylval.node = ast_new_token("AND");add_ast_node(yylval.node); return AND; }
"||"                { yylval.node = ast_new_token("OR"); add_ast_node(yylval.node);return OR; }
"."                 { yylval.node = ast_new_token("DOT"); add_ast_node(yylval.node);return DOT; }
"!"                 { yylval.node = ast_new_token("NOT"); add_ast_node(yylval.node);return NOT; }
"("                 { yylval.node = ast_new_token("LP"); add_ast_node(yylval.node);return LP; }
")"                 { yylval.node = ast_new_token("RP"); add_ast_node(yylval.node);return RP; }
"["                 { yylval.node = ast_new_token("LB"); add_ast_node(yylval.node);return LB; }
"]"                 { yylval.node = ast_new_token("RB"); add_ast_node(yylval.node);return RB; }
"{"                 { yylval.node = ast_new_token("LC"); add_ast_node(yylval.node);return LC; }
"}"                 { yylval.node = ast_new_token("RC"); add_ast_node(yylval.node);return RC; }

.                   { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Mysterious character '%s'\n", yylineno, yytext); 
}

%%

/* 添加AST节点到链表 */
void add_ast_node(ASTNode* node) {
    if (!node) return;
    ASTNodeList* new_node = (ASTNodeList*)malloc(sizeof(ASTNodeList));
    new_node->node = node;
    new_node->next = NULL;  // 新节点指向空
    
    if (!ast_node_list) {
        ast_node_list = new_node;  // 空链表时作为头节点
    } else {
        ASTNodeList* curr = ast_node_list;
        while (curr->next) curr = curr->next;  // 找到尾部
        curr->next = new_node;  // 尾插
    }
}

/* 打印并释放所有AST节点 */
void print_and_free_ast_nodes() {
    ASTNodeList* current = ast_node_list;
    while (current != NULL) {
        ASTNodeList* next = current->next;
        ast_print(current->node, 0);
        ast_free(current->node);
        free(current);
        current = next;
    }
    ast_node_list = NULL;
}

/* 释放所有AST节点而不打印 */
void free_ast_nodes() {
    ASTNodeList* current = ast_node_list;
    while (current != NULL) {
        ASTNodeList* next = current->next;
        ast_free(current->node);
        free(current);
        current = next;
    }
    ast_node_list = NULL;
}
