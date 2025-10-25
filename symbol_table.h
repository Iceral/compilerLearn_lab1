#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Type system (self-contained) ---------- */

typedef struct Type_      Type_;
typedef struct Type_*     Type;

typedef struct FieldList_ FieldList_;
typedef struct FieldList_* FieldList;

struct FieldList_ {
    char name[32];   /* field name */
    Type type;       /* field type */
    FieldList tail;  /* next field */
};

struct Type_ {
    enum {
        BASIC = 0,
        ARRAY = 1,
        STRUCTURE = 2,
        FUNCTION = 3,
        STRUCT_NAME_ = 4
    } kind;
    union {
        /* BASIC */
        int basic; /* 0=int, 1=float (example encoding) */

        /* ARRAY */
        struct {
            Type elem;
            int  size;
        } array_;

        /* STRUCTURE */
        struct {
            FieldList structure; /* field list */
            char*     name;      /* optional struct tag name (nullable) */
        } structure_;

        /* FUNCTION */
        struct {
            Type      returnparam; /* return type */
            int       paramnums;   /* number of parameters */
            FieldList params;      /* parameter types (as a field list) */
        } function;
    } u;
};

/* Deep type equivalence: returns 1 if equal, 0 otherwise */
int typeEqual(Type A, Type B);

/* Strict array equivalence (shape + element types, recursively) */
int check_type_array_strong(Type A, Type B);

/* ---------- Symbol table API ---------- */

typedef struct Symbol {
    char* name;
    Type  type;
    int   kind; /* e.g., VAR / FUNC / STRUCT */
} Symbol;

/* Lifecycle */
void initSymbolTable(void);
void freeSymbolTable(void);

/* Scope management */
void enterScope(void);
/* Returns: 1=ok, 0=attempt to leave global scope, -1=no boundary (corrupt) */
int leaveScope(void);

/* Insert: checks duplicates only in current scope (allows shadowing)
   Returns: 1=ok, 0=duplicate in current scope, -1=error (capacity/oom/invalid) */
int insertSymbol(const char* name, Type type, int kind);

/* Lookup: innermost to outer; returns 1=found (*out set), 0=not found, -1=invalid args */
int lookupSymbol(const char* name, Symbol** out);

#ifdef __cplusplus
}
#endif

#endif /* SYMBOL_TABLE_H */
