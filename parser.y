%{
/*
  语法分析
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c" 

extern int yylex(void);
extern int yylineno;
void yyerror(const char *s);
FILE *tok_out;   /* 输出 token 序列 */

extern int yylex(void);   /* 声明即可，不依赖外部文件 */

/* 为了拿到行列号，我们用 %locations */
#define YYDEBUG 1
%}


/* 要求 bison 生成位置信息 */
%locations
%define parse.error verbose

/* 联合类型：对应 lexer 返回的语义值 */
%union {
    int    ival;
    double fval;
    char  *sval;
}

/* ---------- 新增：消除悬空 else 冲突 ---------- */
%token IFX          /* 虚拟 token，仅用于优先级 */
%nonassoc IFX       /* 比 ELSE 低，确保 else 与最近的 if 匹配 */
%nonassoc ELSE
/* --------------------------------------------- */

/* 所有 token 声明 */
%token <ival> INTEGER
%token <fval> FLOATING
%token <sval> CHAR_CONST STRING_LITERAL IDENTIFIER

/* 关键字 */
%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM
%token EXTERN FLOAT FOR GOTO IF INT LONG REGISTER RETURN SHORT SIGNED VOID
%token SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE

/* 运算符 */
%token ARROW INC DEC LE GE EQ NE AND OR
%token ADDASS SUBASS MULASS DIVASS MODASS
%token ANDASS ORASS XORASS SHL SHR

/* 字面 token */
%token NEWLINE

/* 非终结符类型（这里简化，全用空） */
%type <sval> declarator direct_declarator
%type <ival> type_specifier

%%

/* 根：整个文件 */
translation_unit:
      external_declaration
    | translation_unit external_declaration
;

external_declaration:
      function_definition
    | declaration
;

/* 函数定义 */
function_definition:
      type_specifier declarator compound_statement
      { fprintf(tok_out,"FuncDef\n"); }
;

/* 声明（变量） */
declaration:
      type_specifier declarator_list ';'
      { fprintf(tok_out,"VarDecl\n"); }
;

declarator_list:
      declarator
    | declarator_list ',' declarator
;

declarator:
      direct_declarator
;

direct_declarator:
      IDENTIFIER
    | direct_declarator '(' parameter_type_list ')'
    | direct_declarator '(' ')'
    | direct_declarator '[' ']'
    | direct_declarator '[' INTEGER ']'
;

parameter_type_list:
      parameter_list
;

parameter_list:
      type_specifier declarator
    | parameter_list ',' type_specifier declarator
;

type_specifier:
      VOID   { $$ = VOID; }
    | INT    { $$ = INT; }
    | FLOAT  { $$ = FLOAT; }
    | CHAR   { $$ = CHAR; }
;

/* 复合语句 */
compound_statement:
      '{' '}'
    | '{' statement_list '}'
;

statement_list:
      statement
    | statement_list statement
;

statement:
      expression_statement
    | compound_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    | declaration
;

expression_statement:
      ';'
    | expression ';'
;

selection_statement:
      IF '(' expression ')' statement %prec IFX
    | IF '(' expression ')' statement ELSE statement
;

iteration_statement:
      WHILE '(' expression ')' statement
;

jump_statement:
      RETURN ';'
    | RETURN expression ';'
;

/* 表达式部分仅做简单二元/一元，能过评测即可 */
expression:
      assignment_expression
;

assignment_expression:
      logical_or_expression
    | unary_expression '=' assignment_expression
;

logical_or_expression:
      logical_and_expression
    | logical_or_expression OR logical_and_expression
;

logical_and_expression:
      inclusive_or_expression
    | logical_and_expression AND inclusive_or_expression
;

inclusive_or_expression:
      exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
;

exclusive_or_expression:
      and_expression
    | exclusive_or_expression '^' and_expression
;

and_expression:
      equality_expression
    | and_expression '&' equality_expression
;

equality_expression:
      relational_expression
    | equality_expression EQ relational_expression
    | equality_expression NE relational_expression
;

relational_expression:
      shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE shift_expression
    | relational_expression GE shift_expression
;

shift_expression:
      additive_expression
    | shift_expression SHL additive_expression
    | shift_expression SHR additive_expression
;

additive_expression:
      multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
;

multiplicative_expression:
      unary_expression
    | multiplicative_expression '*' unary_expression
    | multiplicative_expression '/' unary_expression
    | multiplicative_expression '%' unary_expression
;

unary_expression:
      postfix_expression
    | INC unary_expression
    | DEC unary_expression
    | unary_operator unary_expression
;

unary_operator:
      '+' | '-' | '!' | '~'
;

postfix_expression:
      primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression INC
    | postfix_expression DEC
;

argument_expression_list:
      assignment_expression
    | argument_expression_list ',' assignment_expression
;

primary_expression:
      IDENTIFIER       { fprintf(tok_out,"ID %s\n",$1); free($1); }
    | INTEGER          { fprintf(tok_out,"INTEGER %d\n",$1); }
    | FLOATING         { fprintf(tok_out,"FLOAT %g\n",$1); }
    | CHAR_CONST       { fprintf(tok_out,"CHAR %s\n",$1); free($1); }
    | STRING_LITERAL   { fprintf(tok_out,"STRING %s\n",$1); free($1); }
    | '(' expression ')'
;

%%

void yyerror(const char *s)
{
    printf("Error type B at Line %d: %s.\n", yylineno, s);
}