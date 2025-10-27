/* semantic.c — 实验二：语义分析（错误 1~17）
 * 只打印语义错误到 stdout；无错不输出
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tree.h"
#include "symbol_table.h"

/* 符号种类（如你项目已有枚举请替换同名值） */
enum { SYM_VAR = 1, SYM_FUNC = 2, SYM_STRUCT = 3 };

/* 对外 */
int semantic_error_count = 0;
void semanticAnalysis(ASTNode* root);

/* ----------------- 小工具 ----------------- */
static inline const char* N(ASTNode* x){ return x ? x->name : ""; }
static inline ASTNode*    C(ASTNode* x,int i){ return (x && i>=0 && i<x->nchild) ? x->child[i] : NULL; }

static void serr(int tp, int line, const char* fmt, ...) {
    ++semantic_error_count;
    fprintf(stdout, "Error type %d at Line %d: ", tp, line);
    va_list ap; va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fputc('.', stdout);
    fputc('\n', stdout);
}

/* 为避免跨平台 strdup 报错，做个安全版 */
static char* xstrdup(const char* s){ if(!s) return NULL; size_t n=strlen(s)+1; char* p=(char*)malloc(n); if(p) memcpy(p,s,n); return p; }

/* ---------- Type 构造 ---------- */
static Type newType(void){ return (Type)malloc(sizeof(Type_)); }
static Type makeBasicInt(void){ Type t=newType(); t->kind=BASIC; t->u.basic=0; return t; }   /* 0=int */
static Type makeBasicFloat(void){ Type t=newType(); t->kind=BASIC; t->u.basic=1; return t; } /* 1=float */
static Type makeArray(Type elem,int size){ Type t=newType(); t->kind=ARRAY; t->u.array_.elem=elem; t->u.array_.size=size; return t; }

static FieldList makeField(const char* name, Type type, FieldList tail){
    FieldList f=(FieldList)malloc(sizeof(FieldList_));
    memset(f->name,0,sizeof(f->name));
    if(name) strncpy(f->name,name,sizeof(f->name)-1);
    f->type=type; f->tail=tail; return f;
}

static Type makeFunction(Type ret, FieldList params, int paramnums){
    Type t=newType(); t->kind=FUNCTION;
    t->u.function.returnparam=ret;
    t->u.function.params=params;
    t->u.function.paramnums=paramnums;
    return t;
}

/* ---------- Specifier 解析（TYPE / StructSpecifier） ---------- */
static Type parse_StructSpecifier(ASTNode* SS); /* 前置声明 */

static Type fromSpecifier(ASTNode* Spec){
    if(!Spec) return NULL;
    ASTNode* a = C(Spec,0);
    if(!a) return NULL;
    if(strcmp(N(a),"TYPE")==0){
        if(a->sval && strcmp(a->sval,"int")==0)   return makeBasicInt();
        if(a->sval && strcmp(a->sval,"float")==0) return makeBasicFloat();
        return makeBasicInt();
    }
    if(strcmp(N(a),"StructSpecifier")==0) return parse_StructSpecifier(a);
    return NULL;
}

/* ---------- VarDec 完整类型 + 名字 ---------- */
/* VarDec -> ID | VarDec LB INT RB */
static Type buildVarDecType(ASTNode* VarDec, Type base, char** outName){
    if(!VarDec) return NULL;
    ASTNode* a=C(VarDec,0);
    if(strcmp(N(a),"ID")==0){
        if(outName) *outName = a->sval;
        return base;
    }
    /* VarDec -> VarDec LB INT RB */
    Type inner = buildVarDecType(a, base, outName);
    ASTNode* intNode = C(VarDec,2);
    int sz = intNode ? (int)intNode->ival : 0;
    return makeArray(inner, sz);
}

/* ---------- 参数 VarList → FieldList ---------- */
/* VarList -> ParamDec | ParamDec COMMA VarList ;  ParamDec -> Specifier VarDec */
static FieldList buildParamList(ASTNode* VarList, int* outN){
    if(!VarList){ if(outN) *outN=0; return NULL; }
    ASTNode* ParamDec=C(VarList,0);
    ASTNode* Spec=C(ParamDec,0);
    ASTNode* VarDec=C(ParamDec,1);
    Type base=fromSpecifier(Spec);
    char* name=NULL; Type full=buildVarDecType(VarDec, base, &name);
    FieldList head=makeField(name?name:"", full, NULL);
    int cnt=1;
    if(VarList->nchild==3){
        int rest=0; FieldList tail=buildParamList(C(VarList,2), &rest);
        head->tail=tail; cnt+=rest;
    }
    if(outN) *outN=cnt;
    return head;
}

/* ---------- StructSpecifier 解析 ---------- */
/* StructSpecifier -> STRUCT OptTag LC DefList RC | STRUCT Tag */
static FieldList buildFieldListFromDefList(ASTNode* DefList, int* pCount, int* pDupLine, char dupName[32]){
    FieldList head=NULL, *tail=&head; int cnt=0; int dupLine=0; dupName[0]='\0';

    while(DefList && DefList->nchild>0){
        ASTNode* Def=C(DefList,0); /* Def -> Specifier DecList SEMI */
        Type base=fromSpecifier(C(Def,0));
        ASTNode* DecList=C(Def,1);
        while(DecList){
            ASTNode* Dec=C(DecList,0); /* Dec -> VarDec | VarDec ASSIGNOP Exp */
            char* nm=NULL; Type full=buildVarDecType(C(Dec,0), base, &nm);

            /* 结构体内字段名查重（同一个 struct 内） */
            for(FieldList p=head; p; p=p->tail){
                if(strncmp(p->name, nm?nm:"", sizeof(p->name))==0){
                    dupLine = C(Dec,0)->line;
                    strncpy(dupName, nm?nm:"", 31);
                }
            }
            /* 结构体字段不允许指定初值 */
            if (Dec->nchild == 3) {                 // Dec -> VarDec ASSIGNOP Exp
                serr(15, Dec->line, "Field \"%s\" cannot have an initializer in struct", nm?nm:"");
                /* 如果还想继续往下分析，可以把初值子树跳过去 */
            }

            *tail = makeField(nm?nm:"", full, NULL);
            tail = &((*tail)->tail);
            ++cnt;

            if(DecList->nchild==3) DecList=C(DecList,2);
            else break;
        }
        DefList=C(DefList,1);
    }
    if(pCount) *pCount=cnt;
    if(pDupLine) *pDupLine=dupLine;
    return head;
}

static Type parse_StructSpecifier(ASTNode* SS){
    if(!SS) return NULL;
    /* STRUCT Tag —— 使用已定义结构体 */
    if(SS->nchild==2 && strcmp(N(C(SS,0)),"STRUCT")==0 && strcmp(N(C(SS,1)),"Tag")==0){
        ASTNode* id=C(C(SS,1),0); char* sname=id?id->sval:NULL;
        Symbol* sym=NULL;
        if(!lookupSymbol(sname,&sym) || !sym || sym->kind!=SYM_STRUCT){
            serr(17, id?id->line:SS->line, "Undefined structure \"%s\"", sname?sname:"");
            return NULL;
        }
        return sym->type;
    }
    /* STRUCT OptTag LC DefList RC —— 新定义 */
    if(SS->nchild==5 && strcmp(N(C(SS,0)),"STRUCT")==0){
        ASTNode* OptTag=C(SS,1);
        ASTNode* DefList=C(SS,3);
        char* tagName=NULL;
        if(OptTag && OptTag->nchild==1 && strcmp(N(C(OptTag,0)),"ID")==0)
            tagName=C(OptTag,0)->sval;

        int fcnt=0, dupLine=0; char dupName[32];
        FieldList fields=buildFieldListFromDefList(DefList,&fcnt,&dupLine,dupName);
        if(dupLine>0) serr(15, dupLine, "Redefined field \"%s\"", dupName); /* 样例要求 :contentReference[oaicite:6]{index=6} */

        Type t=newType(); t->kind=STRUCTURE;
        t->u.structure_.structure=fields;
        t->u.structure_.name= tagName ? xstrdup(tagName) : NULL;

        if(tagName){
            int ret=insertSymbol(tagName, t, SYM_STRUCT);
            if(ret==0) serr(16, C(OptTag,0)->line, "Duplicated name \"%s\"", tagName); /* :contentReference[oaicite:7]{index=7} */
        }
        return t;
    }
    return NULL;
}

/* ---------- 语句/表达式 前置 ---------- */
static Type typeOfExp(ASTNode* Exp);

/* ---------- 局部定义 ---------- */
static void handle_DefList(ASTNode* DefList){
    while(DefList && DefList->nchild>0){
        ASTNode* Def=C(DefList,0); /* Def -> Specifier DecList SEMI */
        Type base=fromSpecifier(C(Def,0));
        ASTNode* DecList=C(Def,1);

        while(DecList){
            ASTNode* Dec=C(DecList,0); /* Dec -> VarDec | VarDec ASSIGNOP Exp */
            char* name=NULL; Type full=buildVarDecType(C(Dec,0), base, &name);
            int ret=insertSymbol(name, full, SYM_VAR);
            if(ret==0) serr(3, C(Dec,0)->line, "Redefined variable \"%s\"", name?name:""); /* :contentReference[oaicite:8]{index=8} */

            if(Dec->nchild==3){
                Type rhs=typeOfExp(C(Dec,2));
                if(rhs && full && !typeEqual(rhs, full))
                    serr(5, C(Dec,1)->line, "Type mismatched for assignment");
            }
            if(DecList->nchild==3) DecList=C(DecList,2);
            else break;
        }
        DefList=C(DefList,1);
    }
}

/* ---------- 语句/语句序列/复合语句 ---------- */
static void handle_Stmt(ASTNode* Stmt, Type funcRet);

static void handle_StmtList(ASTNode* StmtList, Type funcRet){
    while(StmtList && StmtList->nchild>0){
        handle_Stmt(C(StmtList,0), funcRet);
        StmtList=C(StmtList,1);
    }
}

static void handle_CompSt(ASTNode* CompSt, Type funcRet){
    /* CompSt -> LC DefList StmtList RC */
    enterScope();
    handle_DefList(C(CompSt,1));
    handle_StmtList(C(CompSt,2), funcRet);
    (void)leaveScope();
}

static int isLValue(ASTNode* Exp){
    if(strcmp(N(Exp),"Exp")!=0) return 0;
    if(Exp->nchild==1 && strcmp(N(C(Exp,0)),"ID")==0) return 1;
    if(Exp->nchild==4 && strcmp(N(C(Exp,1)),"LB")==0) return 1;
    if(Exp->nchild==3 && strcmp(N(C(Exp,1)),"DOT")==0) return 1;
    return 0;
}

static void handle_Stmt(ASTNode* Stmt, Type funcRet){
    if(!Stmt) return;
    if(Stmt->nchild==1 && strcmp(N(C(Stmt,0)),"CompSt")==0){ handle_CompSt(C(Stmt,0), funcRet); return; }
    if(Stmt->nchild==2 && strcmp(N(C(Stmt,0)),"Exp")==0){ (void)typeOfExp(C(Stmt,0)); return; }
    if(Stmt->nchild>=3 && strcmp(N(C(Stmt,0)),"RETURN")==0){
        Type t=typeOfExp(C(Stmt,1));
        if(t && funcRet && !typeEqual(t, funcRet))
            serr(8, C(Stmt,1)->line, "Type mismatched for return"); /* :contentReference[oaicite:9]{index=9} */
        return;
    }
    if(Stmt->nchild==5 && strcmp(N(C(Stmt,0)),"IF")==0){
        (void)typeOfExp(C(Stmt,2)); handle_Stmt(C(Stmt,4), funcRet); return;
    }
    if(Stmt->nchild==7 && strcmp(N(C(Stmt,0)),"IF")==0){
        (void)typeOfExp(C(Stmt,2)); handle_Stmt(C(Stmt,4), funcRet); handle_Stmt(C(Stmt,6), funcRet); return;
    }
    if(Stmt->nchild==5 && strcmp(N(C(Stmt,0)),"WHILE")==0){
        (void)typeOfExp(C(Stmt,2)); handle_Stmt(C(Stmt,4), funcRet); return;
    }
}

/* ---------- 表达式类型与检查 ---------- */
static Type typeOfExp(ASTNode* Exp){
    if(!Exp) return NULL;

    /* 赋值：Exp ASSIGNOP Exp */
    if(Exp->nchild==3 && strcmp(N(C(Exp,1)),"ASSIGNOP")==0){
        if(!isLValue(C(Exp,0)))
            serr(6, C(Exp,0)->line, "The left-hand side of an assignment must be a variable");
        Type a=typeOfExp(C(Exp,0)), b=typeOfExp(C(Exp,2));
        if(a && b && !typeEqual(a,b))
            serr(5, C(Exp,1)->line, "Type mismatched for assignment");
        return a?a:b;
    }

    /* 算术：Exp (+|-|*|/) Exp  → 两侧都需 BASIC 且相等 */
    if(Exp->nchild==3 && (
        strcmp(N(C(Exp,1)),"PLUS")==0 || strcmp(N(C(Exp,1)),"MINUS")==0 ||
        strcmp(N(C(Exp,1)),"STAR")==0 || strcmp(N(C(Exp,1)),"DIV")==0 )){
        Type a=typeOfExp(C(Exp,0)), b=typeOfExp(C(Exp,2));
        if(!(a && b && a->kind==BASIC && b->kind==BASIC && typeEqual(a,b)))
            serr(7, C(Exp,1)->line, "Type mismatched for operands"); /* :contentReference[oaicite:10]{index=10} */
        return a?a:b;
    }

    /* 关系/逻辑：结果按 int 处理 */
    if(Exp->nchild==3 && (strcmp(N(C(Exp,1)),"RELOP")==0 ||
                          strcmp(N(C(Exp,1)),"AND")==0  ||
                          strcmp(N(C(Exp,1)),"OR")==0)){
        (void)typeOfExp(C(Exp,0)); (void)typeOfExp(C(Exp,2));
        return makeBasicInt();
    }

    /* 一元：NOT / 负号 */
    if(Exp->nchild==2 && (strcmp(N(C(Exp,0)),"NOT")==0 || strcmp(N(C(Exp,0)),"MINUS")==0)){
        return typeOfExp(C(Exp,1));
    }

    /* 括号： ( Exp ) */
    if(Exp->nchild==3 && strcmp(N(C(Exp,0)),"LP")==0){
        return typeOfExp(C(Exp,1));
    }

    /* 函数调用：ID LP RP | ID LP Args RP */
    if(Exp->nchild==3 && strcmp(N(C(Exp,0)),"ID")==0 && strcmp(N(C(Exp,1)),"LP")==0 && strcmp(N(C(Exp,2)),"RP")==0){
        char* fname=C(Exp,0)->sval;
        Symbol* fs=NULL;
        if(!lookupSymbol(fname,&fs) || !fs){ serr(2, C(Exp,0)->line, "Undefined function \"%s\"", fname); return NULL; }
        if(!fs->type || fs->type->kind!=FUNCTION){ serr(11, C(Exp,0)->line, "\"%s\" is not a function", fname); return NULL; } /* :contentReference[oaicite:11]{index=11} */
        if(fs->type->u.function.paramnums!=0)
            serr(9, Exp->line, "Function \"%s\" is not applicable for arguments", fname);
        return fs->type->u.function.returnparam;
    }
    if(Exp->nchild==4 && strcmp(N(C(Exp,0)),"ID")==0 && strcmp(N(C(Exp,1)),"LP")==0 && strcmp(N(C(Exp,3)),"RP")==0){
        char* fname=C(Exp,0)->sval;
        Symbol* fs=NULL;
        if(!lookupSymbol(fname,&fs) || !fs){ serr(2, C(Exp,0)->line, "Undefined function \"%s\"", fname); return NULL; }
        if(!fs->type || fs->type->kind!=FUNCTION){ serr(11, C(Exp,0)->line, "\"%s\" is not a function", fname); return NULL; }
        /* 收集实参并与形参逐位置比较 */
        FieldList actual=NULL, *tail=&actual; int acnt=0;
        ASTNode* Args=C(Exp,2);
        while(Args){
            Type t=typeOfExp(C(Args,0));
            *tail=makeField("", t, NULL);
            tail=&((*tail)->tail); ++acnt;
            if(Args->nchild==3) Args=C(Args,2);
            else break;
        }
        FieldList pa=actual, pf=fs->type->u.function.params;
        int pos=1;
        while(pa && pf){
            if(!(pa->type && pf->type && typeEqual(pa->type,pf->type)))
                serr(9, Exp->line, "Type mismatched for argument %d", pos);
            pa=pa->tail; pf=pf->tail; ++pos;
        }
        if(acnt != fs->type->u.function.paramnums)
            serr(9, Exp->line, "Function \"%s\" is not applicable for arguments", fname); /* :contentReference[oaicite:12]{index=12} */
        return fs->type->u.function.returnparam;
    }

    /* 数组访问：Exp LB Exp RB */
    if(Exp->nchild==4 && strcmp(N(C(Exp,1)),"LB")==0){
        ASTNode* base=C(Exp,0), *sub=C(Exp,2);
        Type arr=typeOfExp(base);
        if(!(arr && arr->kind==ARRAY)){
            /* 若是 ID，可按样例输出名字 */
            if(base->nchild==1 && strcmp(N(C(base,0)),"ID")==0)
                serr(10, base->line, "\"%s\" is not an array", C(base,0)->sval);      /* :contentReference[oaicite:13]{index=13} */
            else
                serr(10, base->line, "Not an array");
            return NULL;
        }
        Type idx=typeOfExp(sub);
        if(!(idx && idx->kind==BASIC && idx->u.basic==0)){
            /* 若是浮点常数，按样例把字面量打印出来 */
            if(sub->nchild==1 && strcmp(N(C(sub,0)),"FLOAT")==0)
                serr(12, sub->line, "\"%g\" is not an integer", C(sub,0)->fval);      /* :contentReference[oaicite:14]{index=14} */
            else
                serr(12, sub->line, "Array index is not an integer");
        }
        return arr->u.array_.elem;
    }

        /* 结构体成员：Exp DOT ID */
    if (Exp->nchild==3 && strcmp(N(C(Exp,1)),"DOT")==0){
        Type st = typeOfExp(C(Exp,0));
        if (!(st && st->kind==STRUCTURE)) {
            serr(13, C(Exp,1)->line, "Illegal use of \".\"");
            return NULL;
        }
        char* fname = C(Exp,2)->sval;
        FieldList p = st->u.structure_.structure;
        while (p && strncmp(p->name, fname, sizeof(p->name))!=0) p = p->tail;
        if (!p) {
            serr(14, C(Exp,2)->line, "Non-existent field \"%s\"", fname?fname:"");
            return NULL;
        }
        return p->type;
    }

    /* 原子：ID / INT / FLOAT */
    if (Exp->nchild==1){
        ASTNode* a = C(Exp,0);
        if (strcmp(N(a),"ID")==0){
            Symbol* s = NULL;
            if (!lookupSymbol(a->sval, &s) || !s){
                serr(1, a->line, "Undefined variable \"%s\"", a->sval?a->sval:"");
                return NULL;
            }
            return s->type;
        }
        if (strcmp(N(a),"INT")==0)   return makeBasicInt();
        if (strcmp(N(a),"FLOAT")==0) return makeBasicFloat();
    }

    /* 兜底 */
    return NULL;
}

/* ---------- 顶层外部声明 ---------- */
static void handle_ExtDecList(ASTNode* ExtDecList, Type base){
    while (ExtDecList){
        ASTNode* VarDec = C(ExtDecList, 0);
        char* name = NULL;
        Type full = buildVarDecType(VarDec, base, &name);
        int ret = insertSymbol(name, full, SYM_VAR);
        if (ret == 0) {
            serr(3, VarDec->line, "Redefined variable \"%s\"", name?name:"");
        }
        if (ExtDecList->nchild==3) ExtDecList = C(ExtDecList,2);
        else break;
    }
}

/* 为函数体单独做一个入口，便于把形参加入同一作用域 */
static void handle_FuncBody(ASTNode* CompSt, FieldList params, Type funcRet){
    /* CompSt -> LC DefList StmtList RC */
    enterScope();
    /* 形参进入当前作用域（与函数体同级） */
    for (FieldList p=params; p; p=p->tail){
        int r = insertSymbol(p->name, p->type, SYM_VAR);
        if (r == 0) serr(3, CompSt->line, "Redefined variable \"%s\"", p->name);
    }
    handle_DefList(C(CompSt,1));
    handle_StmtList(C(CompSt,2), funcRet);
    (void)leaveScope();
}

static void handle_ExtDef(ASTNode* ExtDef){
    if (!ExtDef) return;
    /* ExtDef -> Specifier FunDec CompSt
             |  Specifier ExtDecList SEMI
             |  Specifier SEMI
     */
    Type base = fromSpecifier(C(ExtDef,0));

    if (ExtDef->nchild==3 && strcmp(N(C(ExtDef,1)),"FunDec")==0){
        ASTNode* FunDec = C(ExtDef,1);
        char* fname = C(FunDec,0)->sval;

        /* FunDec -> ID LP RP | ID LP VarList RP */
        FieldList params = NULL; int pnum = 0;
        if (FunDec->nchild==4 && strcmp(N(C(FunDec,2)),"VarList")==0){
            params = buildParamList(C(FunDec,2), &pnum);
        }

        Type ftype = makeFunction(base, params, pnum);
        int ret = insertSymbol(fname, ftype, SYM_FUNC);
        if (ret == 0) serr(4, C(FunDec,0)->line, "Redefined function \"%s\"", fname?fname:"");

        /* 处理函数体（把形参加入与函数体同一层作用域） */
        handle_FuncBody(C(ExtDef,2), params, base);
        return;
    }

    if (ExtDef->nchild==3 && strcmp(N(C(ExtDef,1)),"ExtDecList")==0){
        handle_ExtDecList(C(ExtDef,1), base);
        return;
    }

    /* Specifier SEMI → 仅声明类型，不做事 */
}

/* ---------- 入口：Program ---------- */
void semanticAnalysis(ASTNode* root){
    if (!root) return;
    initSymbolTable();  /* 建立全局作用域哨兵 */

    /* Program -> ExtDefList */
    ASTNode* ExtDefList = C(root,0);
    while (ExtDefList && ExtDefList->nchild>0){
        ASTNode* ExtDef = C(ExtDefList,0);
        handle_ExtDef(ExtDef);
        ExtDefList = C(ExtDefList,1);
    }
    /* 如需可在此 freeSymbolTable(); */
}

