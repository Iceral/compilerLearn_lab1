#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ========== Symbol table implementation ========== */
/* Stack layout: [NULL(global)][sym...][NULL][sym...] ... */

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
    (void)push_raw(NULL); /* new scope sentinel */
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
        if (symbol_stack[i] == NULL) break; /* stop at sentinel */
        if (strcmp(symbol_stack[i]->name, name) == 0) {
            return 0; /* duplicate in current scope */
        }
    }

    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    if (!s) return -1;

    s->name = xstrdup(name);
    if (!s->name) {
        free(s);
        return -1;
    }

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
        if (!s) continue; /* skip sentinel */
        if (strcmp(s->name, name) == 0) {
            *out = s;
            return 1;
        }
    }
    *out = NULL;
    return 0;
}

/* ========== Type system implementation (self-contained) ========== */

static int typeEqual_internal(Type A, Type B); /* forward */

int check_type_array_strong(Type A, Type B) {
    if (!A || !B) return 0;
    if (A->kind != ARRAY || B->kind != ARRAY) return 0;

    /* sizes must match at each dimension, and element types equal recursively */
    if (A->u.array_.size != B->u.array_.size) return 0;

    Type a_elem = A->u.array_.elem;
    Type b_elem = B->u.array_.elem;

    if (a_elem->kind == ARRAY || b_elem->kind == ARRAY) {
        /* recurse for nested arrays */
        return check_type_array_strong(a_elem, b_elem);
    }
    /* final element: use full type equality */
    return typeEqual_internal(a_elem, b_elem);
}

static int fieldlist_equal(FieldList A, FieldList B) {
    /* Strict structural equality: same length, each field name and type equal in order */
    FieldList pa = A, pb = B;
    while (pa && pb) {
        /* Compare field names; if you want name-insensitive, drop this check */
        if (strncmp(pa->name, pb->name, sizeof(pa->name)) != 0) return 0;
        if (!typeEqual_internal(pa->type, pb->type)) return 0;
        pa = pa->tail;
        pb = pb->tail;
    }
    return (pa == NULL && pb == NULL);
}

static int params_equal(FieldList A, FieldList B) {
    /* Parameter list equality: same length and type equality per position */
    FieldList pa = A, pb = B;
    while (pa && pb) {
        if (!typeEqual_internal(pa->type, pb->type)) return 0;
        pa = pa->tail;
        pb = pb->tail;
    }
    return (pa == NULL && pb == NULL);
}

static int typeEqual_internal(Type A, Type B) {
    if (A == B) return 1;
    if (!A || !B) return 0;
    if (A->kind != B->kind) return 0;

    switch (A->kind) {
    case BASIC:
        return A->u.basic == B->u.basic;

    case ARRAY:
        /* Strong array equality: shape + element types recursively */
        return check_type_array_strong(A, B);

    case STRUCTURE:
        /* Optionally, compare struct tag names if both exist and you want nominal typing:
           if (A->u.structure_.name && B->u.structure_.name)
               if (strcmp(A->u.structure_.name, B->u.structure_.name) != 0) return 0;
           Current choice: purely structural equality by fields. */
        return fieldlist_equal(A->u.structure_.structure, B->u.structure_.structure);

    case FUNCTION:
        if (!typeEqual_internal(A->u.function.returnparam, B->u.function.returnparam)) return 0;
        if (A->u.function.paramnums != B->u.function.paramnums) return 0;
        return params_equal(A->u.function.params, B->u.function.params);

    case STRUCT_NAME_:
        /* If STRUCT_NAME_ represents a named struct alias, you may want to compare names here.
           Fallback: treat like BASIC/nominal id. Without a defined payload, consider unequal unless same pointer. */
        return 0;

    default:
        return 0;
    }
}

int typeEqual(Type A, Type B) {
    return typeEqual_internal(A, B);
}
