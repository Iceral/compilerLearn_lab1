// src/parser/type.c ����ʱ��
#include "type.h"
#include <stdlib.h>

Type make_type(int kind) {
    Type t;
    t.kind = kind;
    return t;
}