#include "mips.h"
#include <stdio.h>

void mips_init(FILE *f)
{
    // 写入.data段
    fprintf(f, ".data\n");
    fprintf(f, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(f, "_ret: .asciiz \"\\n\"\n\n");

    // 写入.globl main和.text段
    fprintf(f, ".globl main\n");
    fprintf(f, ".text\n\n");

    // 实现read函数
    fprintf(f, "read:\n");
    // 打印提示语
    fprintf(f, "    li $v0, 4\n");       // syscall 4表示打印字符串
    fprintf(f, "    la $a0, _prompt\n"); // 加载提示语地址到$a0
    fprintf(f, "    syscall\n");         // 执行系统调用
    // 读取整数
    fprintf(f, "    li $v0, 5\n"); // syscall 5表示读取整数
    fprintf(f, "    syscall\n");   // 执行系统调用
    // 返回
    fprintf(f, "    jr $ra\n\n"); // 跳回到调用处

    // 实现write函数
    fprintf(f, "write:\n");
    // 打印整数
    fprintf(f, "    li $v0, 1\n"); // syscall 1表示打印整数
    // 整数已经在$a0中传入
    fprintf(f, "    syscall\n"); // 执行系统调用
    // 打印换行符
    fprintf(f, "    li $v0, 4\n");    // syscall 4表示打印字符串
    fprintf(f, "    la $a0, _ret\n"); // 加载换行符地址到$a0
    fprintf(f, "    syscall\n");      // 执行系统调用

    fprintf(f, "    move $v0, $0\n");
    // 返回
    fprintf(f, "    jr $ra\n\n"); // 跳回到调用处
}

// 算术运算指令实现
void emit_add(FILE *file, const char *dest_reg, const char *src_reg1, const char *src_reg2)
{
    fprintf(file, "    add %s, %s, %s\n", dest_reg, src_reg1, src_reg2);
}

void emit_sub(FILE *file, const char *dest_reg, const char *src_reg1, const char *src_reg2)
{
    fprintf(file, "    sub %s, %s, %s\n", dest_reg, src_reg1, src_reg2);
}

void emit_mul(FILE *file, const char *dest_reg, const char *src_reg1, const char *src_reg2)
{
    fprintf(file, "    mul %s, %s, %s\n", dest_reg, src_reg1, src_reg2);
}

void emit_div(FILE *file, const char *dest_reg, const char *src_reg1, const char *src_reg2)
{
    fprintf(file, "    div %s, %s, %s\n", dest_reg, src_reg1, src_reg2);
}

// 数据传输指令实现
void emit_lw(FILE *file, const char *reg, int offset, const char *base_reg)
{
    fprintf(file, "    lw %s, %d(%s)\n", reg, offset, base_reg);
}

void emit_sw(FILE *file, const char *reg, int offset, const char *base_reg)
{
    fprintf(file, "    sw %s, %d(%s)\n", reg, offset, base_reg);
}

void emit_move(FILE *file, const char *dest_reg, const char *src_reg)
{
    fprintf(file, "    move %s, %s\n", dest_reg, src_reg);
}

void emit_li(FILE *file, const char *reg, int value)
{
    fprintf(file, "    li %s, %d\n", reg, value);
}

// 跳转与分支指令实现
void emit_j(FILE *file, const char *label)
{
    fprintf(file, "    j %s\n", label);
}

void emit_jal(FILE *file, const char *label)
{
    fprintf(file, "    jal %s\n", label);
}

void emit_jr(FILE *file, const char *reg)
{
    fprintf(file, "    jr %s\n", reg);
}

// 条件分支指令实现
void emit_beq(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    beq %s, %s, %s\n", reg1, reg2, label);
}

void emit_bne(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    bne %s, %s, %s\n", reg1, reg2, label);
}

void emit_bgt(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    bgt %s, %s, %s\n", reg1, reg2, label);
}

void emit_blt(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    blt %s, %s, %s\n", reg1, reg2, label);
}

void emit_bge(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    bge %s, %s, %s\n", reg1, reg2, label);
}

void emit_ble(FILE *file, const char *reg1, const char *reg2, const char *label)
{
    fprintf(file, "    ble %s, %s, %s\n", reg1, reg2, label);
}

// 标号实现
void emit_label(FILE *file, const char *label)
{
    fprintf(file, "%s:\n", label);
}

// 其他指令实现
void emit_addi(FILE *file, const char *dest_reg, const char *src_reg, int imm)
{
    fprintf(file, "    addi %s, %s, %d\n", dest_reg, src_reg, imm);
}

void emit_text_begin(FILE *file) {
    fprintf(file, ".text\n");
}

void emit_syscall(FILE *file) {
    fprintf(file, "    syscall\n");
}

void emit_mips_header(FILE *out) {
   // 正确写法：通过fprintf输出汇编指令到文件
    fprintf(out, ".data\n");
    fprintf(out, "newline: .asciiz \"\\n\"\n");  // 输出汇编的换行符常量
    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");
}