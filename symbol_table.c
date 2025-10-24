#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

/* Implementation: NULL sentinel separates scopes on a linear stack. */

#define MAX_SYMBOLS 1000

static Symbol* symbol_stack[MAX_SYMBOLS];
static int stack_top = -1;

/* Portable strdup */
static char* xstrdup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

/* Push with capacity check */
static int push_raw(Symbol* s) {
    if (stack_top + 1 >= MAX_SYMBOLS) return 0;
    symbol_stack[++stack_top] = s;
    return 1;
}

void initSymbolTable(void) {
    stack_top = -1;
    (void)push_raw(NULL); /* global scope sentinel */
}

void freeSymbolTable(void) {
    while (stack_top >= 0) {
        if (symbol_stack[stack_top] != NULL) {
            free(symbol_stack[stack_top]->name);
            free(symbol_stack[stack_top]);
        }
        --stack_top;
    }
}

void enterScope(void) {
    (void)push_raw(NULL); /* sentinel; if full, later inserts will fail */
}

int leaveScope(void) {
    /* Locate nearest sentinel */
    int i = stack_top;
    while (i >= 0 && symbol_stack[i] != NULL) --i;
    if (i < 0) return -1; /* no boundary found: corrupt */
    if (i == 0) return 0; /* cannot leave global scope */

    /* Free current scope symbols */
    while (stack_top > i) {
        free(symbol_stack[stack_top]->name);
        free(symbol_stack[stack_top]);
        --stack_top;
    }
    /* Pop sentinel */
    --stack_top;
    return 1;
}

int insertSymbol(const char* name, Type type, int kind) {
    if (!name) return -1;

    /* Capacity precheck */
    if (stack_top + 1 >= MAX_SYMBOLS) return -1;

    /* Check duplicates within current scope only */
    for (int i = stack_top; i >= 0; --i) {
        if (symbol_stack[i] == NULL) break;
        if (strcmp(symbol_stack[i]->name, name) == 0) {
            return 0; /* duplicate in current scope */
        }
    }

    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    if (!s) return -1;

    s->name = xstrdup(name);
    if (!s->name) { free(s); return -1; }

    s->type = type;
    s->kind = kind;

    if (!push_raw(s)) {
        free(s->name);
        free(s);
        return -1;
    }
    return 1;
}

int lookupSymbol(const char* name, Symbol** out) {
    if (!name || !out) return -1;
    for (int i = stack_top; i >= 0; --i) {
        Symbol* s = symbol_stack[i];
        if (!s) continue; /* skip sentinels */
        if (strcmp(s->name, name) == 0) {
            *out = s;
            return 1;
        }
    }
    *out = NULL;
    return 0;
}

int typeEqual(Type A,Type B){
	FieldList A_f=A->u.structure_.structure;
	FieldList B_f=B->u.structure_.structure;
	if(A==B){
		return 1;
	}
	else{
		if(A==NULL){
			printf("Type A is NULL\n");
			assert(0);
			return 0;
		}else if(B==NULL){
			printf("Type B is NULL\n");
			assert(0);
			return 0;
		}
		if(A->kind!=B->kind){
			return 0;
		}
		else{
			switch (A->kind){
				case BASIC:{
					return A->u.basic==B->u.basic;
					break;}
				case ARRAY:{
					int result2=0;
					int cnt1=0;int cnt2=0;
					Type temp1=A;Type temp2=B;
					while(temp1!=NULL){
						temp1=temp1->u.array_.elem;
						cnt1+=1;
						if(temp1->kind!=ARRAY)break;
					}
					while(temp2!=NULL){
						temp2=temp2->u.array_.elem;
						cnt2+=1;
						if(temp2->kind!=ARRAY)break;
					}
					result2=(cnt1==cnt2);
					return result2;
					break;}
				case STRUCTURE:{
					PR("HERE\n");
					FieldList A_f=A->u.structure_.structure;
					FieldList B_f=B->u.structure_.structure;
					while(A_f!=NULL&&B_f!=NULL){
						A_f=A_f->tail;
						B_f=B_f->tail;
						;
					}
					if(A_f!=NULL||B_f!=NULL){
						return 0;
					}
					A_f=A->u.structure_.structure;
					B_f=B->u.structure_.structure;
					int flag=0;
					while(A_f!=NULL&&B_f!=NULL){
						if(A_f->type->kind!=B_f->type->kind){
							return 0;
						}
						if(A_f->type->kind==ARRAY){
							int array_result=check_type_array_strong(A_f->type,B_f->type);
							if(array_result==0)return 0;
							else return 1;
						}else{
							if(check_type(A_f->type,B_f->type)==0){
								return 0;
							};
						}
						A_f=A_f->tail;
						B_f=B_f->tail;
						;
					}
					return 1;
					break;
				}
				case FUNCTION:{
					if(A->u.function.paramnums!=B->u.function.paramnums){
						return 0;//函数参数个数不一样;
					}
					if(check_type(A->u.function.returnparam,B->u.function.returnparam)==0){
						return 0;
					}
					FieldList A_f=A->u.function.params;
					FieldList B_f=B->u.function.params;
					int flag=0;
					while(A_f!=NULL&&B_f!=NULL){
						if(check_type(A_f->type,B_f->type)==0){
							return 0;
						};
						A_f=A_f->tail;
						B_f=B_f->tail;						
					}
					return 1;
					break;
				}
				default:
					printf("check_type bug,A->kind cant't find!\n");assert(0);return 0;
			}

		}
	}

	return 0;
}

int check_type_array_strong(Type A,Type B){
	if(A->u.array_.size!=B->u.array_.size){
		return 0;
	}
	int result1;
	if(A->u.array_.elem->kind!=B->u.array_.elem->kind){
		return 0;
	}
	if(A->u.array_.elem->kind==ARRAY){
		result1=check_type_array_strong(A->u.array_.elem,B->u.array_.elem);
	}else{
	result1=check_type(A->u.array_.elem,B->u.array_.elem);}
	return result1;
}
