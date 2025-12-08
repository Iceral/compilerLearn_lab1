#ifndef CODEGEN_H
#define CODEGEN_H
#include "ir.h"
#include "mips.h"

#include <stdio.h>

// 初始化寄存器分配系统
void regalloc_init();

// 结束寄存器分配，清理资源
void regalloc_finish();

// 获取给定变量的寄存器
int get_reg(const char* var_name);

// 打印当前寄存器映射（可选，用于调试）
void print_reg_map();

// 清理栈信息
void clear_stack_info();

// 新增：声明find_stack_offset
int find_stack_offset(const char *var_name);

// 核心翻译函数：输入中间代码链表头 + 输出文件指针
void generate_mips(InterCodes head, FILE *out);
void generate_mips_code(InterCodes head, FILE *out);
#endif