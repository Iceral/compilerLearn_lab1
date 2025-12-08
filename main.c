#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"
#include "ir.h"
#include "codegen.h"

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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.cmm> [output.s]\n", argv[0]);
        fprintf(stderr, "  If output.s is given and ends with .s, generate MIPS assembly\n");
        fprintf(stderr, "  Otherwise, print intermediate code\n");
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = (argc >= 3) ? argv[2] : NULL;

    FILE* f = fopen(input_file, "r");
    if (!f) {
        perror(input_file);
        return 1;
    }

    yyrestart(f);
    yyparse();  // 直接调用，不需要返回值了（全局变量会标记错误）
    fclose(f);

    // 有词法/语法错误，直接退出
    if (lexical_error_occurred || syntaxError) {
        fprintf(stderr, "Compilation terminated due to lexical/syntax errors.\n");
        return 1;
    }

    // 语义分析（实验四可以注释掉）
    // semanticAnalysis(ast_root);
    // if (semantic_error_count > 0) {
    //     fprintf(stderr, "Compilation terminated due to %d semantic error(s).\n", semantic_error_count);
    //     return 1;
    // }

    // 翻译为中间代码
    ir_init();
    translate_ast(ast_root);
    clear_array_info();

    // 输出阶段
    FILE* out = stdout;
    int close_out = 0;

    if (output_file) {
        out = fopen(output_file, "w");
        if (!out) {
            perror(output_file);
            ir_free_all();
            return 1;
        }
        close_out = 1;
    }

    if (output_file && has_suffix(output_file, ".s")) {
        // 生成 MIPS 汇编
        generate_mips(ir_head, out);
        printf("MIPS assembly written to %s\n", output_file);
    } else {
        // 默认打印中间代码（带格式更好看）
        fprintf(out, "========== Intermediate Code ==========\n");
        ir_print_all(out);
        if (output_file)
            printf("Intermediate code written to %s\n", output_file);
    }

    if (close_out) fclose(out);
    ir_free_all();

    if (ast_root) {
        ast_free(ast_root);
        ast_root = NULL;
    }

    printf("Compilation successful!\n");
    return 0;
}
