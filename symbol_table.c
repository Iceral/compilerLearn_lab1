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
