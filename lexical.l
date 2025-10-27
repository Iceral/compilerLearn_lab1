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

/* 错误标志 */
int lexical_error_occurred = 0;
ASTNodeList* ast_node_list = NULL; 
ASTNode* ast_root = NULL;
%}

/* 正则别名（修复转义错误和规则定义） */
digit           [0-9]
letter          [a-zA-Z_]

/* 整数正则（无修改，已正确） */
decimal         ([1-9]{digit}*|0)
octal           0[0-7]+                
hex             0[xX][0-9a-fA-F]+      
illegal_octal   0[0-7]*[89]            
illegal_hex     0[xX]([0-9a-fA-F]*[g-zG-Z]|[xX]*)  

/* 浮点数正则（无修改，已正确） */
float_common    ([0-9]+\.[0-9]+|[0-9]*\.[0-9]+|[0-9]+\.)  
exp_part        [eE][+-]?{digit}+      
exp_float       {float_common}{exp_part} 
non_exp_float   {float_common}          
illegal_float   {float_common}[eE][+-]?[a-zA-Z_]*  

id              {letter}({letter}|{digit}){0,31}
single_comment  "//".*
multi_comment   "/\*"([^*]|\*+[^*/])*\*+"/"
unclosed_multi  "/\*"([^*]|\*+[^*/])*
whitespace      [ \t\r\n]

%%

{whitespace}            { /* 忽略空白符 */ }

{single_comment}    { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Unsupported single-line comment\n", yylineno); 
}
{multi_comment}     { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Unsupported multi-line comment\n", yylineno); 
}
{unclosed_multi}    { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Unclosed multi-line comment\n", yylineno); 
}

"int"               { 
    yylval.node = ast_new_type("int");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return TYPE; 
}
"float"             { 
    yylval.node = ast_new_type("float");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return TYPE; 
}
"struct"            { 
    yylval.node = ast_new_token("STRUCT");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return STRUCT; 
}
"return"            { 
    yylval.node = ast_new_token("RETURN");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return RETURN; 
}
"if"                { 
    yylval.node = ast_new_token("IF");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return IF; 
}
"else"              { 
    yylval.node = ast_new_token("ELSE");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return ELSE; 
}
"while"             { 
    yylval.node = ast_new_token("WHILE");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno; 
    add_ast_node(yylval.node);
    return WHILE; 
}

{id}                {    
    yylval.node = ast_new_id(yytext); 
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node);
    return ID; 
}

{illegal_octal}     { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal octal number '%s'\n", yylineno, yytext); 
}
{illegal_hex}       { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal hexadecimal number '%s'\n", yylineno, yytext); 
}
{hex}               { 
    yylval.node = ast_new_int(strtoll(yytext, NULL, 16));
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return INT; 
}
{octal}             { 
    yylval.node = ast_new_int(strtoll(yytext, NULL, 8));
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return INT; 
}
{decimal}           { 
    yylval.node = ast_new_int(strtoll(yytext, NULL, 10));
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return INT; 
}

{exp_float}         { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Unsupported exponential floating-point number '%s'\n", yylineno, yytext); 
}
{illegal_float}     { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Illegal floating-point number '%s'\n", yylineno, yytext); 
}
{non_exp_float}     { 
    yylval.node = ast_new_float(atof(yytext));
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return FLOAT; 
}

";"                 { 
    yylval.node = ast_new_token("SEMI");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return SEMI; 
}
","                 { 
    yylval.node = ast_new_token("COMMA");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return COMMA; 
}
"="                 { 
    yylval.node = ast_new_token("ASSIGNOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return ASSIGNOP; 
}
">="                { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
"<="                { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
"=="                { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
"!="                { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
">"                 { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
"<"                 { 
    yylval.node = ast_new_token("RELOP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RELOP; 
}
"+"                 { 
    yylval.node = ast_new_token("PLUS");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return PLUS; 
}
"-"                 { 
    yylval.node = ast_new_token("MINUS");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return MINUS; 
}
"*"                 { 
    yylval.node = ast_new_token("STAR");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return STAR; 
}
"/"                 { 
    yylval.node = ast_new_token("DIV");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return DIV; 
}
"&&"                { 
    yylval.node = ast_new_token("AND");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return AND; 
}
"||"                { 
    yylval.node = ast_new_token("OR");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return OR; 
}
"."                 { 
    yylval.node = ast_new_token("DOT");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return DOT; 
}
"!"                 { 
    yylval.node = ast_new_token("NOT");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return NOT; 
}
"("                 { 
    yylval.node = ast_new_token("LP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return LP; 
}
")"                 { 
    yylval.node = ast_new_token("RP");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RP; 
}
"["                 { 
    yylval.node = ast_new_token("LB");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return LB; 
}
"]"                 { 
    yylval.node = ast_new_token("RB");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RB; 
}
"{"                 { 
    yylval.node = ast_new_token("LC");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return LC; 
}
"}"                 { 
    yylval.node = ast_new_token("RC");
    ast_set_line(yylval.node, yylineno);
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    add_ast_node(yylval.node); 
    return RC; 
}

.                   { 
    lexical_error_occurred = 1;
    printf("Error type A at Line %d: Mysterious character '%s'\n", yylineno, yytext); 
}

%%

void add_ast_node(ASTNode* node) {
    if (!node) return;
    ASTNodeList* new_node = (ASTNodeList*)malloc(sizeof(ASTNodeList));
    new_node->node = node;
    new_node->next = NULL;
    
    if (!ast_node_list) {
        ast_node_list = new_node;
    } else {
        ASTNodeList* curr = ast_node_list;
        while (curr->next) curr = curr->next;
        curr->next = new_node;
    }
}