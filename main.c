#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"
#include "ir.h"

/* IR 限制相关标志（来自 semantic.c） */
extern int has_struct_var_or_param;
extern int has_multidim_array_or_array_param;

/* 这些符号由 Flex/Bison 提供 */
extern FILE* yyin;
extern void yyrestart(FILE* f);
extern int yyparse(void);

/* 在 syntax.y 中定义并赋值的全局根节点 */
extern ASTNode* ast_root;
extern int syntaxError;
extern int lexical_error_occurred;

/* 语义分析（在 semantic.c 中实现） */
extern void semanticAnalysis(ASTNode* root);
extern int  semantic_error_count;

/* IR 翻译（在 translate_ext.c 中实现） */
extern void translate_ast(ASTNode* root);
extern void clear_array_info(void);  // ← 新增：清理数组信息表

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <input.cmm> [output.ir]\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) { 
        perror(argv[1]); 
        return 1; 
    }

    yyrestart(f);
    int ret = yyparse();
    fclose(f);

    /* 若词法/语法有错误，直接返回 */
    if (lexical_error_occurred || syntaxError || ret != 0) {
        return 0;
    }

    /* 进行语义分析（如果需要的话） */
    semanticAnalysis(ast_root);

     /* ---------- 限制选做 3.1 / 3.2 ---------- */

    if (has_struct_var_or_param) {
        printf("Cannot translate: Code contains variables or parameters of structure type.\n");
        /* 不生成 IR，直接退出 main */
        if (ast_root) { ast_free(ast_root); ast_root = NULL; }
        return 0;
    }

    if (has_multidim_array_or_array_param) {
        printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
        /* 不生成 IR，直接退出 main */
        if (ast_root) { ast_free(ast_root); ast_root = NULL; }
        return 0;
    }
    /* ---------- 若未触发限制，则正常生成 IR ---------- */
    // 初始化 IR 模块
    ir_init();

    // 进行 IR 生成
    translate_ast(ast_root);

    // ✅ 清理数组信息表
    clear_array_info();

    // 如果提供了输出文件参数
    if (argc >= 3) {
        FILE* out = fopen(argv[2], "w");
        if (!out) {
            perror("Error opening output file");
            ir_free_all();
            return 1;
        }
            
        // 打印 IR 到文件
        ir_print_all(out);
        fclose(out);
    } else {
        // 否则输出到 stdout
        ir_print_all(stdout);
    }

    // 清理 IR 内存
    ir_free_all();

    /* 释放 AST */
    if (ast_root) {
        ast_free(ast_root);
        ast_root = NULL;
    }

    return 0;
}