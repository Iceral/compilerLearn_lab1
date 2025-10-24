// src/parser/type.h （临时调试用）
#ifndef TYPE_H
#define TYPE_H

// 临时定义 Type 为一个简单结构（仅用于编译通过和基础测试）
typedef struct Type_ {
    int kind;  // 0=int, 1=float, 2=array, 3=struct...
    // 其他字段暂时不需要，调试时只关心“能存类型”
} Type;

// 提供一个方便创建 Type 的函数（可选）
Type make_type(int kind);

#endif