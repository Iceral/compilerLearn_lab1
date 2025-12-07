#include "regalloc.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define REG_COUNT 10  // 假设有10个寄存器 ($t0-$t9)
static const char *reg_names[] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"
};

// 寄存器使用状态：0 表示空闲，1 表示占用
static int reg_usage[REG_COUNT];

// 栈偏移管理
#define STACK_BASE_OFFSET 4  // 栈基址偏移
static int stack_offset = STACK_BASE_OFFSET;  // 初始化为 4，跳过 $ra
typedef struct {
    char var_name[64];       // 变量名
    int stack_offset;        // 栈偏移（相对于 $fp）
} StackMap;

static StackMap *stack_map = NULL;  // 栈映射表
static int stack_map_size = 0;

// 初始化寄存器分配系统
void regalloc_init() {
    // 初始化寄存器使用状态
    for (int i = 0; i < REG_COUNT; i++) {
        reg_usage[i] = 0;  // 所有寄存器初始化为空闲状态
    }

    // 初始化栈管理
    stack_offset = STACK_BASE_OFFSET;
    stack_map_size = 0;
    stack_map = NULL;
}


// 结束寄存器分配，清理资源
void regalloc_finish() {
    // 重置寄存器使用状态
    for (int i = 0; i < REG_COUNT; i++) {
        reg_usage[i] = 0;
    }

    // 清理栈映射表
    if (stack_map != NULL) {
        free(stack_map);
        stack_map = NULL;
    }
}

// 获取给定变量的寄存器
int get_reg(const char* var_name) {
    // 如果有空闲寄存器，直接返回
    for (int i = 0; i < REG_COUNT; i++) {
        if (reg_usage[i] == 0) {
            reg_usage[i] = 1;  // 标记寄存器为已占用
            return i;  // 返回寄存器索引
        }
    }
    // 如果没有空闲寄存器，返回栈溢出处理
    int stack_offset = find_stack_offset(var_name);
    return -1;  // 返回 -1 表示需要栈溢出处理
}

// 打印当前寄存器映射（可选，用于调试）
void print_reg_map() {
    printf("寄存器使用状态：\n");
    for (int i = 0; i < REG_COUNT; i++) {
        printf("$t%d: %s\n", i, reg_usage[i] == 0 ? "空闲" : "占用");
    }
}


// 清理栈信息
void clear_stack_info() {
    if (stack_map != NULL) {
        free(stack_map);
        stack_map = NULL;
    }
    stack_map_size = 0;
}

// 辅助函数：查找变量的栈偏移（溢出时使用）
int find_stack_offset(const char *var_name) {
    for (int i = 0; i < stack_map_size; i++) {
        if (strcmp(stack_map[i].var_name, var_name) == 0) {
            return stack_map[i].stack_offset;
        }
    }
    // 未找到则分配新栈偏移
    stack_map = realloc(stack_map, (stack_map_size + 1) * sizeof(StackMap));
    strncpy(stack_map[stack_map_size].var_name, var_name, sizeof(stack_map[stack_map_size].var_name)-1);
    stack_map[stack_map_size].stack_offset = stack_offset;
    stack_offset += 4;  // 每个变量占4字节
    stack_map_size++;
    return stack_map[stack_map_size-1].stack_offset;
}