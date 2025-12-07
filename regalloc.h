#ifndef REGALLOC_H
#define REGALLOC_H

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

#endif // REGALLOC_H
