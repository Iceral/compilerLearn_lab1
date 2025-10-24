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

#ifdef __cplusplus
}
#endif

#endif /* SYMBOL_TABLE_H */
