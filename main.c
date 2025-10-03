#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "syntax.tab.h"

/* 这些符号由 Flex/Bison 提供 */
extern FILE* yyin;
extern void yyrestart(FILE* f);
extern int yyparse(void);

/* 在 syntax.y 中定义并赋值的全局根节点 */
extern ASTNode* g_root;

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

    /* 按实验要求：若存在词法/语法错误，只输出错误信息，且不打印语法树。
       这里假设：
         - 词法错误：由词法器直接按格式打印（A 类），yyparse 可能仍返回 0/非 0；
         - 语法错误：yyerror 按格式打印（B 类），yyparse 非 0。
       最稳妥做法：仅当 ret==0 且 g_root 非空时才打印语法树。 */
    if (ret == 0 && g_root) {
        ast_print(g_root, 0);
        ast_free(g_root);
        g_root = NULL;
    }

    return 0;
}
