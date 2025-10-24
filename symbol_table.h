#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "type.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Symbol exposed so callers can read fields returned by lookup. */
typedef struct Symbol {
    char* name;
    Type  type;
    int   kind; /* e.g., VAR/FUNC/STRUCT */
} Symbol;

typedef struct FieldList_* FieldList;
typedef struct Type_* Type;

struct FieldList_{
	char name[32];//域的名字;和树节点名字长度一样;
	Type type;//域的类型;
	FieldList tail;//下一个域
};

struct Type_{
	enum { BASIC=0,ARRAY=1,STRUCTURE=2,FUNCTION=3,STRUCT_NAME_=4} kind;
	union{
		//基本类型
		int basic;//0表示int,1表示float;
		//数组类型信息包括元素类型与数组大小构成
		struct{ Type elem;int size;}array_;
		//结构体类型的信息是一个链表
		struct{FieldList structure;char *name;}structure_;
		struct{
			Type returnparam;//返回的参数类型
			int paramnums;//输入的参数数量
			FieldList params;//函数的具体参数;
			}function;
}u;
};
/* Lifecycle */
void initSymbolTable(void);
void freeSymbolTable(void);

/* Scope management */
void enterScope(void);
/* Returns: 1=ok, 0=attempt to leave global scope, -1=corrupt (no boundary) */
int leaveScope(void);

/* Insert: checks duplicates only in current scope (allows shadowing)
   Returns: 1=ok, 0=duplicate in current scope, -1=error (capacity/oom/invalid) */
int insertSymbol(const char* name, Type type, int kind);

/* Lookup: innermost to outer
   Returns: 1=found (*out set), 0=not found, -1=invalid args */
int lookupSymbol(const char* name, Symbol** out);

int typeEqual(Type A,Type B);

int check_type_array_strong(Type A,Type B);
#ifdef __cplusplus
}
#endif

#endif /* SYMBOL_TABLE_H */
