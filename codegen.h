#ifndef CODEGEN_H
#define CODEGEN_H
#include "ir.h"
#include "mips.h"
#include "regalloc.h"  // 引入B部分的寄存器分配接口
#include <stdio.h>

// 核心翻译函数：输入中间代码链表头 + 输出文件指针
void generate_mips(InterCodes head, FILE *out);
void generate_mips_code(InterCodes head, FILE *out);
#endif