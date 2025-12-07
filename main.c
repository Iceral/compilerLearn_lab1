#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"
#include "ir.h"
#include "codegen.h"   // ★★ 新增：生成 MIPS 的接口

extern FILE* yyin;
extern void yyrestart(FILE* f);
extern int yyparse(void);

extern ASTNode* ast_root;
extern int syntaxError;
extern int lexical_error_occurred;

extern void semanticAnalysis(ASTNode* root);
extern int  semantic_error_count;

extern void translate_ast(ASTNode* root);
extern void clear_array_info(void);

int has_suffix(const char* name, const char* suf) {
    int n1 = strlen(name), n2 = strlen(suf);
    if (n1 < n2) return 0;
    return strcmp(name + n1 - n2, suf) == 0;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s input.cmm output.s\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }

    yyrestart(f);
    int ret = yyparse();
    fclose(f);

    if (lexical_error_occurred || syntaxError || ret != 0) {
        return 0;
    }

    // semanticAnalysis(ast_root);

    if (semantic_error_count == 0) {

        ir_init();
        translate_ast(ast_root);
        clear_array_info();

        if (argc >= 3) {
            FILE* out = fopen(argv[2], "w");
            if (!out) {
                perror(argv[2]);
                ir_free_all();
                return 1;
            }

            if (has_suffix(argv[2], ".s")) {
                // ★★ 用户要求输出 MIPS 汇编
                generate_mips(ir_head, out);
            } else {
                // 默认打印 IR
                ir_print_all(out);
            }

            fclose(out);
        } else {
            // 默认输出 IR 到 stdout
            ir_print_all(stdout);
        }

        ir_free_all();
    }

    if (ast_root) {
        ast_free(ast_root);
        ast_root = NULL;
    }

    return 0;
}
