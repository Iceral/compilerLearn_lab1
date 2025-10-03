%option yylineno
%option noyywrap
%option nounput
%option noinput

%{
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
#ifndef YYSTYPE
typedef union {
    ASTNode* node;
} YYSTYPE;
YYSTYPE yylval;
#endif

/* 错误标志和临时文件 */
int lexical_error_occurred = 0;
FILE* temp_output = NULL;
char temp_filename[] = "/tmp/ast_output_XXXXXX";

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
    fprintf(stdout, "Error type A at Line %d: Unclosed multi-line comment\n", yylineno); 
}

{whitespace}        { /* 丢弃空白符 */ }

"int"               { yylval.node = ast_new_type("int"); return TYPE; }
"float"             { yylval.node = ast_new_type("float"); return TYPE; }
"struct"            { yylval.node = ast_new_token("STRUCT"); return STRUCT; }
"return"            { yylval.node = ast_new_token("RETURN"); return RETURN; }
"if"                { yylval.node = ast_new_token("IF"); return IF; }
"else"              { yylval.node = ast_new_token("ELSE"); return ELSE; }
"while"             { yylval.node = ast_new_token("WHILE"); return WHILE; }

{id}                { yylval.node = ast_new_id(yytext); return ID; }

{illegal_octal}     { 
    lexical_error_occurred = 1;
    fprintf(stdout, "Error type A at Line %d: Illegal octal number '%s'\n", yylineno, yytext); 
}
{illegal_hex}       { 
    lexical_error_occurred = 1;
    fprintf(stdout, "Error type A at Line %d: Illegal hexadecimal number '%s'\n", yylineno, yytext); 
}
{hex}               { yylval.node = ast_new_int(strtoll(yytext, NULL, 16)); return INT; }
{octal}             { yylval.node = ast_new_int(strtoll(yytext, NULL, 8)); return INT; }
{decimal}           { yylval.node = ast_new_int(strtoll(yytext, NULL, 10)); return INT; }

{illegal_float}     { 
    lexical_error_occurred = 1;
    fprintf(stdout, "Error type A at Line %d: Illegal floating point number '%s'\n", yylineno, yytext); 
}
{float_const}       { yylval.node = ast_new_float(atof(yytext)); return FLOAT; }

";"                 { yylval.node = ast_new_token("SEMI"); return SEMI; }
","                 { yylval.node = ast_new_token("COMMA"); return COMMA; }
"="                 { yylval.node = ast_new_token("ASSIGNOP"); return ASSIGNOP; }
">="                { yylval.node = ast_new_token("RELOP"); return RELOP; }
"<="                { yylval.node = ast_new_token("RELOP"); return RELOP; }
"=="                { yylval.node = ast_new_token("RELOP"); return RELOP; }
"!="                { yylval.node = ast_new_token("RELOP"); return RELOP; }
">"                 { yylval.node = ast_new_token("RELOP"); return RELOP; }
"<"                 { yylval.node = ast_new_token("RELOP"); return RELOP; }
"+"                 { yylval.node = ast_new_token("PLUS"); return PLUS; }
"-"                 { yylval.node = ast_new_token("MINUS"); return MINUS; }
"*"                 { yylval.node = ast_new_token("STAR"); return STAR; }
"/"                 { yylval.node = ast_new_token("DIV"); return DIV; }
"&&"                { yylval.node = ast_new_token("AND"); return AND; }
"||"                { yylval.node = ast_new_token("OR"); return OR; }
"."                 { yylval.node = ast_new_token("DOT"); return DOT; }
"!"                 { yylval.node = ast_new_token("NOT"); return NOT; }
"("                 { yylval.node = ast_new_token("LP"); return LP; }
")"                 { yylval.node = ast_new_token("RP"); return RP; }
"["                 { yylval.node = ast_new_token("LB"); return LB; }
"]"                 { yylval.node = ast_new_token("RB"); return RB; }
"{"                 { yylval.node = ast_new_token("LC"); return LC; }
"}"                 { yylval.node = ast_new_token("RC"); return RC; }

.                   { 
    lexical_error_occurred = 1;
    fprintf(stdout, "Error type A at Line %d: Mysterious character '%s'\n", yylineno, yytext); 
}

%%

/* 将文件内容复制到标准输出 */
void copy_file_to_stdout(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }
    fclose(file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stdout, "Usage: %s <C--_file>\n", argv[0]);
        return 1;
    }
    
    FILE* f = fopen(argv[1], "r");
    if (!f) { 
        perror(argv[1]); 
        return 1; 
    }
    yyin = f;
    
    /* 创建临时文件 */
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) {
        perror("mkstemp");
        fclose(f);
        return 1;
    }
    temp_output = fdopen(temp_fd, "w");
    if (!temp_output) {
        perror("fdopen");
        close(temp_fd);
        unlink(temp_filename);
        fclose(f);
        return 1;
    }
    
    /* 保存原来的stdout */
    FILE* original_stdout = stdout;
    
    /* 将stdout重定向到临时文件 */
    stdout = temp_output;
    
    int token;
    while ((token = yylex()) != 0) {
        /* 继续处理所有token，不要因为错误而中断 */
        /* 只有在没有错误的情况下才打印AST节点 */
        if (!lexical_error_occurred && yylval.node) {
            ast_print(yylval.node, 0);
            ast_free(yylval.node);
        } else if (yylval.node) {
            /* 有错误时仍然要释放节点内存 */
            ast_free(yylval.node);
        }
    }
    
    /* 恢复stdout */
    stdout = original_stdout;
    fclose(temp_output);
    fclose(f);
    
    /* 根据是否有错误决定输出 */
    if (!lexical_error_occurred) {
        /* 没有错误，将临时文件内容输出到标准输出 */
        copy_file_to_stdout(temp_filename);
    }
    /* 如果有错误，什么都不输出（错误信息已经输出到stdout） */
    
    /* 清理临时文件 */
    unlink(temp_filename);
    
    return lexical_error_occurred ? 1 : 0;
}

