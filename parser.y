/*
 * 语法分析器定义文件（基于Bison）
 * 功能：解析类C语言的语法结构，生成抽象语法树（AST），并检测语法错误
 * 支持特性：变量声明、函数定义、结构体、条件/循环语句、表达式运算等
 * 错误处理：统计语法错误数量，输出错误行号及提示
 */

%{
// -------------------------- 头文件与全局变量声明 --------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "lex.yy.c"
#include "tree.h"

// 外部变量：从词法分析器（lex.yy.c）导入的行号计数器
extern int yylineno;

// 语法分析状态标志
int emptyflag = 0;        // 空产生式标记
int emptystart = 0;       // 空产生式起始行号
int syntaxError = 0;      // 语法错误计数器

int yyerror(const char *msg);

// AST全局根节点
ASTNode* ast_root;

// AST节点操作函数声明
ASTNode *create_ast_node(const char *node_name, int line_num);
void add_child_nodes(ASTNode *parent, int arg_count, ...);
void traverse_ast(const ASTNode *curr_node, int depth);

// 词法分析器入口函数（从lex.yy.c导入）
extern int yylex(void);
%}


// -------------------------- Bison核心配置 --------------------------
// 定义语义值类型：使用联合体存储AST节点指针
%union {
    struct ASTNode *node;  // 语义值为AST节点指针
}

// 启用位置跟踪：获取每个语法单元的行号信息（@$表示当前单元位置）
%locations

// -------------------------- 终结符（Token）定义 --------------------------
// 所有Token均关联AST节点指针（<node>），从词法分析器生成
%token  <node> INT       // 整型常量（如123）
%token  <node> FLOAT     // 浮点型常量（如3.14）
%token  <node> ID        // 标识符（如变量名、函数名）
%token  <node> SEMI      // 分号（;）
%token  <node> COMMA     // 逗号（,）
%token  <node> ASSIGNOP  // 赋值运算符（=）
%token  <node> RELOP     // 关系运算符（>、<、>=、<=、==、!=）
%token  <node> PLUS      // 加法运算符（+）
%token  <node> MINUS     // 减法运算符（-）
%token  <node> STAR      // 乘法运算符（*）
%token  <node> DIV       // 除法运算符（/）
%token  <node> AND       // 逻辑与（&&）
%token  <node> OR        // 逻辑或（||）
%token  <node> DOT       // 点运算符（.，用于结构体成员访问）
%token  <node> NOT       // 逻辑非（!）
%token  <node> TYPE      // 类型关键字（int、float、struct）
%token  <node> LP        // 左括号（(）
%token  <node> RP        // 右括号（)）
%token  <node> LB        // 左方括号（[）
%token  <node> RB        // 右方括号（]）
%token  <node> LC        // 左花括号（{）
%token  <node> RC        // 右花括号（}）
%token  <node> STRUCT    // 结构体关键字（struct）
%token  <node> RETURN    // 返回语句关键字（return）
%token  <node> IF        // 条件语句关键字（if）
%token  <node> ELSE      // 条件语句关键字（else）
%token  <node> WHILE     // 循环语句关键字（while）
%type <node> ExtDecList

// -------------------------- 非终结符定义 --------------------------
// 所有非终结符均关联AST节点指针（<node>），对应语法单元
%type <node> Program     // 程序入口（根节点）
%type <node> ExtDefList  // 外部定义列表（全局变量/函数定义）
%type <node> ExtDef      // 外部定义（单个全局变量/函数）
%type <node> Specifier   // 类型说明符（int、float、struct）
%type <node> StructSpecifier  // 结构体说明符（struct定义）
%type <node> OptTag      // 可选标签（结构体定义中的可选名称）
%type <node> Tag         // 标签（结构体引用时的名称）
%type <node> VarDec      // 变量声明符（变量名+数组维度）
%type <node> FunDec      // 函数声明符（函数名+参数列表）
%type <node> VarList     // 函数参数列表（多个参数声明）
%type <node> ParamDec    // 参数声明（单个参数的类型+变量名）
%type <node> CompSt      // 复合语句（{...}包裹的语句块）
%type <node> StmtList    // 语句列表（复合语句中的多个语句）
%type <node> Stmt        // 语句（表达式语句、条件语句等）
%type <node> DefList     // 定义列表（复合语句中的变量定义）
%type <node> Def         // 定义（单个变量定义）
%type <node> DecList     // 声明列表（多个变量声明）
%type <node> Dec         // 声明（单个变量的声明+可选赋值）
%type <node> Exp         // 表达式（算术/关系/逻辑表达式）
%type <node> Args        // 函数实参列表（调用函数时的参数）

// -------------------------- 语法优先级与结合性 --------------------------
// 从低到高定义优先级，解决语法二义性（如if-else、运算符优先级）
%right ASSIGNOP          // 赋值运算符（右结合，如a=b=c）
%left OR                 // 逻辑或（左结合，优先级低于赋值）
%left AND                // 逻辑与（左结合，优先级低于逻辑或）
%left RELOP              // 关系运算符（左结合，优先级低于逻辑与）
%left PLUS MINUS         // 加减运算符（左结合，优先级低于关系运算）
%left STAR DIV           // 乘除运算符（左结合，优先级低于加减）
%right NOT               // 逻辑非（右结合，优先级高于乘除）
%right UMINUS            // 负号（右结合，如-123，优先级高于逻辑非）
%left DOT                // 结构体成员访问（左结合，优先级高于负号）
%left LB RB              // 数组下标访问（左结合，优先级高于成员访问）
%left LP RP              // 函数调用/表达式分组（左结合，优先级高于数组）

// if-else优先级：else与最近的if匹配（解决悬空else问题）
%nonassoc LOWER_THAN_ELSE  // 标记if的优先级低于else
%nonassoc ELSE             // else优先级高于if

// 语法入口点：从Program开始解析
%start Program

%%
// -------------------------- 语法规则定义（按语法单元分类） --------------------------

// 1. 程序入口与外部定义（全局层级）
// 程序根节点：仅包含外部定义列表
Program: ExtDefList 
{
    // 创建Program节点，行号为外部定义列表的起始行
    $$ = create_ast_node("Program", @$.first_line);
    // 为Program节点添加子节点（ExtDefList）
    add_child_nodes($$, 1, $1);
    // 设置AST根节点
    ast_root = $$;
};

// 外部定义列表：多个外部定义的递归组合（或空）
ExtDefList: ExtDef ExtDefList 
{
    $$ = create_ast_node("ExtDefList", @$.first_line);
    add_child_nodes($$, 2, $1, $2);  // 子节点：当前ExtDef + 剩余ExtDefList
}
| /* 空产生式：无外部定义 */
{
    $$ = NULL;  // 空列表无需创建节点
    emptystart = yylineno;  // 记录空列表的起始行号
};

// 外部定义：3种形式（全局变量声明、空类型声明、函数定义）
ExtDef: Specifier ExtDecList SEMI 
{
    // 形式1：类型说明符 + 外部声明列表 + 分号（如int a, b[10];）
    $$ = create_ast_node("ExtDef", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Specifier SEMI 
{
    // 形式2：类型说明符 + 分号（如struct S;  forward声明）
    $$ = create_ast_node("ExtDef", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| Specifier FunDec CompSt 
{
    // 形式3：类型说明符 + 函数声明 + 复合语句（如int add(int a) { return a; }）
    $$ = create_ast_node("ExtDef", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| error SEMI 
{
    // 错误处理1：任意错误 + 分号（如int a b;）
    syntaxError++;  // 错误计数+1
}
| Specifier error SEMI 
{
    // 错误处理2：类型说明符 + 错误 + 分号（如int ;）
    syntaxError++;
}
| error Specifier SEMI 
{
    // 错误处理3：错误 + 类型说明符 + 分号（如a int;）
    syntaxError++;
};

// 外部声明列表：单个变量声明（或多个声明用逗号分隔）
ExtDecList: VarDec 
{
    // 单个变量声明（如a）
    $$ = create_ast_node("ExtDecList", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| VarDec COMMA ExtDecList 
{
    // 多个变量声明（如a, b, c[5]）
    $$ = create_ast_node("ExtDecList", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| VarDec error ExtDefList 
{
    // 错误处理：变量声明 + 错误 + 外部定义列表（如a b int c;）
    syntaxError++;
};


// 2. 类型说明符（基础类型与结构体）
// 类型说明符：基础类型（TYPE）或结构体（StructSpecifier）
Specifier: TYPE 
{
    // 基础类型（如int、float）
    $$ = create_ast_node("Specifier", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| StructSpecifier 
{
    // 结构体类型（如struct S { int x; }）
    $$ = create_ast_node("Specifier", @$.first_line);
    add_child_nodes($$, 1, $1);
};

// 结构体说明符：2种形式（带成员定义、仅引用标签）
StructSpecifier: STRUCT OptTag LC DefList RC 
{
    // 形式1：struct + 可选标签 + { 定义列表 }（如struct S { int x; }）
    $$ = create_ast_node("StructSpecifier", @$.first_line);
    add_child_nodes($$, 5, $1, $2, $3, $4, $5);
}
| STRUCT Tag 
{
    // 形式2：struct + 标签（引用已定义的结构体，如struct S）
    $$ = create_ast_node("StructSpecifier", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
};

// 可选标签：有标签（ID）或无标签（空）
OptTag: ID 
{
    // 有标签（如struct S中的S）
    $$ = create_ast_node("OptTag", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| /* 空产生式：无标签（如struct { int x; }） */
{
    $$ = NULL;  // 空标签无需创建节点
};

// 标签：仅标识符（引用已定义的结构体标签）
Tag: ID 
{
    $$ = create_ast_node("Tag", @$.first_line);
    add_child_nodes($$, 1, $1);
};


// 3. 变量与函数声明（变量名、函数名、参数）
// 变量声明符：标识符（或标识符+数组维度）
VarDec: ID 
{
    // 普通变量（如a）
    $$ = create_ast_node("VarDec", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| VarDec LB INT RB 
{
    // 数组变量（如a[10]、b[2][3]）
    $$ = create_ast_node("VarDec", @$.first_line);
    add_child_nodes($$, 4, $1, $2, $3, $4);
}
| VarDec LB error RB 
{
    // 错误处理：数组下标错误（如a[b]中的b不是常量）
    syntaxError++;
};

// 函数声明符：函数名+参数列表（或无参数）
FunDec: ID LP VarList RP 
{
    // 带参数的函数（如add(int a, int b)）
    $$ = create_ast_node("FunDec", @$.first_line);
    add_child_nodes($$, 4, $1, $2, $3, $4);
}
| ID LP RP 
{
    // 无参数的函数（如foo()）
    $$ = create_ast_node("FunDec", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| ID LP error RP 
{
    // 错误处理：参数列表错误（如add(int a b)）
    syntaxError++;
}
| error LP VarList RP 
{
    // 错误处理：函数名错误（如123(int a)）
    syntaxError++;
};

// 函数参数列表：单个参数声明（或多个参数用逗号分隔）
VarList: ParamDec COMMA VarList 
{
    // 多个参数（如int a, float b）
    $$ = create_ast_node("VarList", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| ParamDec 
{
    // 单个参数（如int a）
    $$ = create_ast_node("VarList", @$.first_line);
    add_child_nodes($$, 1, $1);
};

// 参数声明：类型说明符 + 变量声明符（如int a、float b[5]）
ParamDec: Specifier VarDec 
{
    $$ = create_ast_node("ParamDec", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
};


// 4. 语句与复合语句（函数体、条件/循环语句）
// 复合语句：{ 定义列表 语句列表 }（函数体、if/while块）
CompSt: LC DefList StmtList RC 
{
    $$ = create_ast_node("CompSt", @$.first_line);
    add_child_nodes($$, 4, $1, $2, $3, $4);
};

// 语句列表：单个语句（或多个语句递归组合）
StmtList: Stmt StmtList 
{
    // 多个语句（如a=1; return a;）
    $$ = create_ast_node("StmtList", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| /* 空产生式：无语句（如{}） */
{
    $$ = NULL;  // 空语句列表无需创建节点
};


// 语句：支持6种常见语句类型 + 错误处理
Stmt: Exp SEMI 
{
    // 1. 表达式语句（如a = 1 + 2;、foo(3);）
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| CompSt 
{
    // 2. 复合语句（嵌套的{...}块，如if/while的循环体）
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| RETURN Exp SEMI 
{
    // 3. 返回语句（如return a;、return 0;）
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE 
{
    // 4. 无else的if语句（如if(a > 0) { ... }）
    // %prec LOWER_THAN_ELSE：强制if优先级低于else，避免else匹配歧义
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 5, $1, $2, $3, $4, $5);
}
| IF LP Exp RP Stmt ELSE Stmt 
{
    // 5. 带else的if语句（如if(a > 0) { ... } else { ... }）
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 7, $1, $2, $3, $4, $5, $6, $7);
}
| WHILE LP Exp RP Stmt 
{
    // 6. while循环语句（如while(a > 0) { ... }）
    $$ = create_ast_node("Stmt", @$.first_line);
    add_child_nodes($$, 5, $1, $2, $3, $4, $5);
}
| error SEMI 
{
    // 错误处理1：任意错误+分号（如a b;、123;）
    syntaxError++;
}
| Exp error SEMI 
{
    // 错误处理2：表达式不完整（如a + ;、a = ;）
    syntaxError++;
}
| RETURN Exp error 
{
    // 错误处理3：return语句缺少分号（如return a）
    syntaxError++;
}
| RETURN error SEMI 
{
    // 错误处理4：return后无表达式（如return ;）
    syntaxError++;
};


// 5. 变量定义（复合语句内的局部变量）
// 定义列表：单个定义（或多个定义递归组合）
DefList: Def DefList 
{
    $$ = create_ast_node("DefList", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| /* 空产生式：无局部变量定义 */
{
    $$ = NULL;
};

// 定义：类型说明符 + 声明列表 + 分号（局部变量定义）
Def: Specifier DecList SEMI 
{
    $$ = create_ast_node("Def", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Specifier error SEMI 
{
    // 错误处理1：类型后声明错误（如int ;、int a b;）
    syntaxError++;
}
| Specifier DecList error 
{
    // 错误处理2：定义缺少分号（如int a = 1）
    syntaxError++;
};

// 声明列表：单个声明（或多个声明用逗号分隔）
DecList: Dec 
{
    $$ = create_ast_node("DecList", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| Dec COMMA DecList 
{
    // 多个声明（如a = 1, b[5] = {0}, c）
    $$ = create_ast_node("DecList", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
};

// 声明：变量声明符（或变量声明符+初始化赋值）
Dec: VarDec 
{
    // 无初始化的声明（如int a;、float b[10];）
    $$ = create_ast_node("Dec", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| VarDec ASSIGNOP Exp 
{
    // 带初始化的声明（如int a = 1 + 2;、float b = 3.14;）
    $$ = create_ast_node("Dec", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
};


// 6. 表达式（算术/关系/逻辑运算、函数调用等）
// 表达式：按优先级从低到高定义，覆盖所有运算场景
Exp: Exp ASSIGNOP Exp 
{
    // 1. 赋值表达式（如a = b、a = b = c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp AND Exp 
{
    // 2. 逻辑与表达式（如a > 0 && b < 10）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp OR Exp 
{
    // 3. 逻辑或表达式（如a == 0 || b == 1）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp RELOP Exp 
{
    // 4. 关系表达式（如a > 5、b <= 3、c == 0）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp PLUS Exp 
{
    // 5. 加法表达式（如a + 3、b + c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp MINUS Exp 
{
    // 6. 减法表达式（如a - 5、b - c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp STAR Exp 
{
    // 7. 乘法表达式（如a * 4、b * c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp DIV Exp 
{
    // 8. 除法表达式（如a / 2、b / c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| LP Exp RP 
{
    // 9. 括号表达式（强制改变优先级，如(a + b) * c）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| MINUS Exp %prec UMINUS 
{
    // 10. 负号表达式（如-3、-a，%prec UMINUS指定为单目运算符）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| NOT Exp 
{
    // 11. 逻辑非表达式（如!a、!(b > 0)）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 2, $1, $2);
}
| ID LP Args RP 
{
    // 12. 带参数的函数调用（如add(a, b)、foo(1 + 2)）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 4, $1, $2, $3, $4);
}
| ID LP RP 
{
    // 13. 无参数的函数调用（如foo()、init()）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp LB Exp RB 
{
    // 14. 数组下标访问（如a[5]、b[i + 1]）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 4, $1, $2, $3, $4);
}
| Exp DOT ID 
{
    // 15. 结构体成员访问（如s.x、student.age）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| ID 
{
    // 16. 标识符表达式（变量名，如a、b）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| INT 
{
    // 17. 整型常量表达式（如123、0、-456）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 1, $1);
}
| FLOAT 
{
    // 18. 浮点型常量表达式（如3.14、0.5、-2.718）
    $$ = create_ast_node("Exp", @$.first_line);
    add_child_nodes($$, 1, $1);
}
// 以下为表达式相关错误处理
| Exp ASSIGNOP error 
{
    // 错误1：赋值表达式右侧不完整（如a = ）
    syntaxError++;
}
| Exp AND error 
{
    // 错误2：逻辑与右侧不完整（如a && ）
    syntaxError++;
}
| Exp OR error 
{
    // 错误3：逻辑或右侧不完整（如a || ）
    syntaxError++;
}
| Exp RELOP error 
{
    // 错误4：关系表达式右侧不完整（如a > ）
    syntaxError++;
}
| Exp PLUS error 
{
    // 错误5：加法表达式右侧不完整（如a + ）
    syntaxError++;
}
| Exp MINUS error 
{
    // 错误6：减法表达式右侧不完整（如a - ）
    syntaxError++;
}
| Exp STAR error 
{
    // 错误7：乘法表达式右侧不完整（如a * ）
    syntaxError++;
}
| Exp DIV error 
{
    // 错误8：除法表达式右侧不完整（如a / ）
    syntaxError++;
}
| LP error RP 
{
    // 错误9：括号内表达式错误（如(a b)）
    syntaxError++;
}
| MINUS error 
{
    // 错误10：负号后无表达式（如- ）
    syntaxError++;
}
| NOT error 
{
    // 错误11：逻辑非后无表达式（如! ）
    syntaxError++;
}
| ID LP error RP 
{
    // 错误12：函数参数列表错误（如foo(a b)）
    syntaxError++;
}
| Exp LB error RB 
{
    // 错误13：数组下标错误（如a[ ]、a[b c]）
    syntaxError++;
};

// 函数实参列表：单个表达式（或多个表达式用逗号分隔）
Args: Exp COMMA Args 
{
    // 多个实参（如add(a + b, 3, c)）
    $$ = create_ast_node("Args", @$.first_line);
    add_child_nodes($$, 3, $1, $2, $3);
}
| Exp 
{
    // 单个实参（如foo(a)、bar(1 + 2)）
    $$ = create_ast_node("Args", @$.first_line);
    add_child_nodes($$, 1, $1);
};

%%

// -------------------------- 辅助函数实现 --------------------------
// 1. 语法错误处理函数（Bison默认调用yyerror）
int yyerror(const char *msg) {
    syntaxError++;
    // 输出错误行号和提示（msg为Bison生成的错误描述）
    fprintf(stderr, "Error type B at Line %d: %s.\n", yylineno, msg);
    return 0;
}

// 2. 创建AST节点：初始化节点名称、行号，清空子节点/兄弟节点指针

ASTNode *create_ast_node(const char *node_name, int line_num) {
    ASTNode *new_node = malloc(sizeof(ASTNode));
    new_node->name = strdup(node_name);  // 注意：const char*，不能 strncpy
    new_node->line = line_num;
    new_node->kind = ASTK_NONTERM;
    new_node->nchild = 0;
    new_node->child = NULL;
    return new_node;
}

// 3. 为父节点添加子节点：支持可变参数（多个子节点）
void add_child_nodes(ASTNode *parent, int arg_count, ...) {
    if (!parent || arg_count <= 0) return;

    parent->nchild = arg_count;
    parent->child = malloc(sizeof(ASTNode*) * arg_count);

    va_list args;
    va_start(args, arg_count);
    for (int i = 0; i < arg_count; i++) {
        parent->child[i] = va_arg(args, ASTNode*);
    }
    va_end(args);
}

// 4. 遍历并打印AST（递归实现，按缩进显示层级关系）
void traverse_ast(const ASTNode *curr_node, int depth) {
    if (curr_node == NULL) return;          /* 递归终止 */

    /* 1. 打印缩进 */
    for (int i = 0; i < depth; ++i) printf("  ");

    /* 2. 打印节点名 */
    printf("%s", curr_node->name);

    /* 3. 打印附加信息（仅对叶子或特定 kind） */
    switch (curr_node->kind) {
        case ASTK_INT:
            printf(": %lld", curr_node->ival);
            break;
        case ASTK_FLOAT:
            printf(": %.6f", curr_node->fval);   /* 保留 6 位小数，与原代码一致 */
            break;
        case ASTK_ID:
        case ASTK_TYPE:
            printf(": %s", curr_node->sval);
            break;
        default:
            /* 非终结符或其他情况，只打印行号（当 line > 0） */
            if (curr_node->line > 0)
                printf(" (Line %d)", curr_node->line);
            break;
    }
    printf("\n");

    /* 4. 递归访问子结点 */
    for (int i = 0; i < curr_node->nchild; ++i)
        traverse_ast(curr_node->child[i], depth + 1);
}