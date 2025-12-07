#ifndef MIPS_H_
#define MIPS_H_

#include <stdio.h>

// 初始化函数
void mips_init(FILE* file);

// 算术运算指令
void emit_add(FILE* file, const char* dest_reg, const char* src_reg1, const char* src_reg2);
void emit_sub(FILE* file, const char* dest_reg, const char* src_reg1, const char* src_reg2);
void emit_mul(FILE* file, const char* dest_reg, const char* src_reg1, const char* src_reg2);
void emit_div(FILE* file, const char* dest_reg, const char* src_reg1, const char* src_reg2);

// 数据传输指令
void emit_lw(FILE* file, const char* reg, int offset, const char* base_reg);
void emit_sw(FILE* file, const char* reg, int offset, const char* base_reg);
void emit_move(FILE* file, const char* dest_reg, const char* src_reg);
void emit_li(FILE* file, const char* reg, int value);

// 跳转与分支指令
void emit_j(FILE* file, const char* label);
void emit_jal(FILE* file, const char* label);
void emit_jr(FILE* file, const char* reg);

// 条件分支指令
void emit_beq(FILE* file, const char* reg1, const char* reg2, const char* label);
void emit_bne(FILE* file, const char* reg1, const char* reg2, const char* label);
void emit_bgt(FILE* file, const char* reg1, const char* reg2, const char* label);
void emit_blt(FILE* file, const char* reg1, const char* reg2, const char* label);
void emit_bge(FILE* file, const char* reg1, const char* reg2, const char* label);
void emit_ble(FILE* file, const char* reg1, const char* reg2, const char* label);

// 标号
void emit_label(FILE* file, const char* label);

// 其他指令
void emit_addi(FILE* file, const char* dest_reg, const char* src_reg, int imm);

#endif /* MIPS_H_ */