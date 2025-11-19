#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"
#include "ir.h"  // ← 新增：包含 IR 头文件

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

/* IR 翻译（假设你在 translate_ext.c 中实现了它） */
extern void translate_ast(ASTNode* root);

void print_relop_nodes(ASTNode* node) {
    if (!node) return;
    if (node->kind == ASTK_TOKEN && strcmp(node->name, "RELOP") == 0) {
        printf("Found RELOP: sval='%s', line=%d\n", 
               node->sval ? node->sval : "(null)", node->line);
    }
    for (int i = 0; i < node->nchild; i++) {
        print_relop_nodes(node->child[i]);
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <input.cmm>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }

    yyrestart(f);
    int ret = yyparse();   /* 0 = success; nonzero = syntax error */
    fclose(f);

    /* --- 实验二的判定逻辑 ---
       1) 若词法/语法有任意错误：它们已经各自按格式输出到 stdout 了；
          这里什么也不做（不打印语法树、不做语义分析）。
       2) 只有当词法/语法均无错时，才进行语义分析；
          语义分析只在有错时输出错误，无错不输出任何内容。
    */
    if (lexical_error_occurred || syntaxError || ret != 0) {
        return 0;
    }

    /* 进行语义分析（内部会按“Error type X at Line N: ...”打印到 stdout） */
    // semanticAnalysis(ast_root);

    /* 【新增】如果语义分析无错，生成并输出 IR */
    if (semantic_error_count == 0) {
        // 初始化 IR 模块
        ir_init();

        // 进行 IR 生成
        translate_ast(ast_root);

        // 打开输出文件（写模式）
        FILE* out = fopen(argv[2], "w");
        if (!out) {
            perror("Error opening output file");
            ir_free_all();  // 出现文件打开错误时，确保释放 IR 资源
            return 1;
        }

        // 打印 IR 到文件
        ir_print_all(out);

        // 关闭输出文件
        fclose(out);

        // 清理 IR 内存
        ir_free_all();
    }

    /* 实验二不需要打印语法树；释放 AST 即可（可选） */
    if (ast_root) {
        ast_free(ast_root);
        ast_root = NULL;
    }

    return 0;
}
