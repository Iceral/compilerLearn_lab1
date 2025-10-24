// src/parser/type.c £®¡Ÿ ±£©
#include "type.h"
#include <stdlib.h>

Type make_type(int kind) {
    Type t;
    t.kind = kind;
    return t;
}