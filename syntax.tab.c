/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 10 "./parser.y"

// -------------------------- 头文件与全局变量声明 --------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "lex.yy.c"
#include "tree.h"

// 外部变量：从词法分析器（lex.yy.c）导入的行号计数器
extern int yylineno;

// 语法分析状态标志
int emptyflag = 0;        // 空产生式标记
int emptystart = 0;       // 空产生式起始行号
int syntaxError = 0;      // 语法错误计数器

int yyerror(const char *msg);

// AST全局根节点
ASTNode* ast_root;

// AST节点操作函数声明
ASTNode *create_ast_node(const char *node_name, int line_num);
void add_child_nodes(ASTNode *parent, int arg_count, ...);
void traverse_ast(const ASTNode *curr_node, int depth);

// 词法分析器入口函数（从lex.yy.c导入）
extern int yylex(void);

#line 102 "./syntax.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_SEMI = 6,                       /* SEMI  */
  YYSYMBOL_COMMA = 7,                      /* COMMA  */
  YYSYMBOL_ASSIGNOP = 8,                   /* ASSIGNOP  */
  YYSYMBOL_RELOP = 9,                      /* RELOP  */
  YYSYMBOL_PLUS = 10,                      /* PLUS  */
  YYSYMBOL_MINUS = 11,                     /* MINUS  */
  YYSYMBOL_STAR = 12,                      /* STAR  */
  YYSYMBOL_DIV = 13,                       /* DIV  */
  YYSYMBOL_AND = 14,                       /* AND  */
  YYSYMBOL_OR = 15,                        /* OR  */
  YYSYMBOL_DOT = 16,                       /* DOT  */
  YYSYMBOL_NOT = 17,                       /* NOT  */
  YYSYMBOL_TYPE = 18,                      /* TYPE  */
  YYSYMBOL_LP = 19,                        /* LP  */
  YYSYMBOL_RP = 20,                        /* RP  */
  YYSYMBOL_LB = 21,                        /* LB  */
  YYSYMBOL_RB = 22,                        /* RB  */
  YYSYMBOL_LC = 23,                        /* LC  */
  YYSYMBOL_RC = 24,                        /* RC  */
  YYSYMBOL_STRUCT = 25,                    /* STRUCT  */
  YYSYMBOL_RETURN = 26,                    /* RETURN  */
  YYSYMBOL_IF = 27,                        /* IF  */
  YYSYMBOL_ELSE = 28,                      /* ELSE  */
  YYSYMBOL_WHILE = 29,                     /* WHILE  */
  YYSYMBOL_UMINUS = 30,                    /* UMINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 31,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_Program = 33,                   /* Program  */
  YYSYMBOL_ExtDefList = 34,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 35,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 36,                /* ExtDecList  */
  YYSYMBOL_Specifier = 37,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 38,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 39,                    /* OptTag  */
  YYSYMBOL_Tag = 40,                       /* Tag  */
  YYSYMBOL_VarDec = 41,                    /* VarDec  */
  YYSYMBOL_FunDec = 42,                    /* FunDec  */
  YYSYMBOL_VarList = 43,                   /* VarList  */
  YYSYMBOL_ParamDec = 44,                  /* ParamDec  */
  YYSYMBOL_CompSt = 45,                    /* CompSt  */
  YYSYMBOL_StmtList = 46,                  /* StmtList  */
  YYSYMBOL_Stmt = 47,                      /* Stmt  */
  YYSYMBOL_DefList = 48,                   /* DefList  */
  YYSYMBOL_Def = 49,                       /* Def  */
  YYSYMBOL_DecList = 50,                   /* DecList  */
  YYSYMBOL_Dec = 51,                       /* Dec  */
  YYSYMBOL_Exp = 52,                       /* Exp  */
  YYSYMBOL_Args = 53                       /* Args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   422

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  152

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   131,   131,   142,   148,   154,   160,   166,   172,   177,
     182,   189,   195,   201,   210,   216,   224,   230,   238,   245,
     250,   259,   265,   271,   278,   284,   290,   295,   302,   308,
     316,   325,   332,   339,   345,   351,   357,   363,   370,   376,
     382,   387,   392,   397,   406,   412,   417,   422,   427,   434,
     439,   447,   453,   463,   469,   475,   481,   487,   493,   499,
     505,   511,   517,   523,   529,   535,   541,   547,   553,   559,
     565,   572,   577,   582,   587,   592,   597,   602,   607,   612,
     617,   622,   627,   632,   639,   645
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "INT", "FLOAT", "ID", "SEMI",
  "COMMA", "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV", "AND",
  "OR", "DOT", "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC", "STRUCT",
  "RETURN", "IF", "ELSE", "WHILE", "UMINUS", "LOWER_THAN_ELSE", "$accept",
  "Program", "ExtDefList", "ExtDef", "ExtDecList", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "VarDec", "FunDec", "VarList",
  "ParamDec", "CompSt", "StmtList", "Stmt", "DefList", "Def", "DecList",
  "Dec", "Exp", "Args", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-132)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-34)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       5,     4,  -132,    11,    75,  -132,    62,    19,  -132,  -132,
      82,    67,    77,  -132,  -132,  -132,     2,    79,  -132,    95,
     105,    84,  -132,    39,  -132,    39,    85,  -132,    90,    99,
      52,    39,  -132,    46,    89,    39,    99,   107,   121,   111,
    -132,   122,  -132,  -132,  -132,   119,   128,    55,   137,     7,
      93,   149,  -132,  -132,   134,  -132,    39,  -132,  -132,  -132,
    -132,   151,  -132,  -132,   139,   148,   165,   170,   187,   142,
     145,  -132,   153,    55,   108,  -132,   284,  -132,  -132,    99,
    -132,  -132,   143,  -132,    81,  -132,    81,   152,   326,   172,
     124,   284,   284,  -132,  -132,   173,  -132,   192,   209,   214,
     231,   236,   253,   258,   275,   175,   280,   368,  -132,   163,
    -132,   297,   166,  -132,  -132,  -132,  -132,  -132,   340,   354,
    -132,  -132,   368,  -132,   401,  -132,    33,  -132,    33,  -132,
      81,  -132,    81,  -132,   394,  -132,   381,  -132,   177,   311,
    -132,   284,  -132,    66,    66,  -132,  -132,  -132,   157,  -132,
      66,  -132
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    14,    19,     0,     2,     0,     0,    15,     8,
       0,    20,     0,    17,     1,     3,     0,    21,     6,     0,
       0,     0,    10,    45,     9,     0,     0,     5,     0,     0,
       0,    45,     7,     0,     0,    45,     0,     0,    29,     0,
      25,     0,    13,    21,    12,     0,     0,     0,     0,    51,
       0,    49,    16,    44,    30,    27,     0,    26,    24,    23,
      22,     0,    69,    70,    68,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    47,     0,    48,    46,     0,
      28,    40,     0,    80,    62,    81,    63,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    50,     0,
      65,    85,     0,    79,    61,    43,    42,    36,     0,     0,
      41,    71,    53,    74,    56,    75,    57,    76,    58,    77,
      59,    78,    60,    72,    54,    73,    55,    67,     0,     0,
      82,     0,    64,     0,     0,    83,    66,    84,    37,    39,
       0,    38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -132,     3,  -132,   171,    17,  -132,  -132,  -132,   -29,
    -132,   -12,  -132,   180,   129,  -131,    30,  -132,   115,  -132,
     -65,    64
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    19,     7,     8,    12,    13,    20,
      21,    37,    38,    71,    72,    73,    34,    35,    50,    51,
      74,   112
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      84,    86,    88,    90,    49,    -4,     1,    54,    24,    15,
       9,   107,   148,   149,    41,    76,    11,   111,    10,   151,
      16,    25,     2,     2,    17,    18,   118,   119,    30,     3,
       3,    42,   122,   124,   126,   128,   130,   132,   134,   136,
      33,   139,    36,    36,    80,   101,   102,    48,    33,   105,
      49,    43,    33,    45,   106,    46,    61,     2,    62,    63,
      64,    47,    -4,     1,     3,    53,    65,    61,    -4,    62,
      63,    64,    66,    36,    67,    14,   111,    65,    31,   -33,
       2,    68,    69,    66,    70,    67,    39,     3,    22,    31,
     -18,     1,    68,    69,    77,    70,    -4,   105,    26,    78,
      23,    27,   106,     2,    43,    40,    28,    31,     2,    95,
       3,   -11,    29,    52,    96,     3,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   116,    30,    55,    56,   106,
     117,    57,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    59,    58,    75,   109,   106,    62,    63,    64,    83,
      60,    62,    63,    64,    65,    30,    79,    81,    82,    65,
      66,    91,    67,   110,    92,    66,    85,    67,    62,    63,
      64,    87,   113,    62,    63,    64,    65,    93,   115,   120,
     137,    65,    66,   140,    67,   150,   142,    66,    89,    67,
      62,    63,    64,   121,   108,    62,    63,    64,    65,   145,
      44,    32,    94,    65,    66,   147,    67,     0,     0,    66,
     123,    67,    62,    63,    64,   125,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    65,    66,     0,    67,     0,
       0,    66,   127,    67,    62,    63,    64,   129,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    65,    66,     0,
      67,     0,     0,    66,   131,    67,    62,    63,    64,   133,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    65,
      66,     0,    67,     0,     0,    66,   135,    67,    62,    63,
      64,   138,     0,    62,    63,    64,    65,    62,    63,    64,
       0,    65,    66,     0,    67,    65,     0,    66,     0,    67,
       0,    66,     0,    67,   141,    97,    98,    99,   100,   101,
     102,   103,   104,   105,     0,     0,     0,     0,   106,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
       0,     0,   106,   146,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,     0,     0,   114,   106,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,     0,     0,
     143,   106,    97,    98,    99,   100,   101,   102,   103,   104,
     105,     0,     0,     0,   144,   106,    97,    98,    99,   100,
     101,   102,   103,   104,   105,     0,     0,     0,     0,   106,
      98,    99,   100,   101,   102,   103,     0,   105,     0,     0,
       0,     0,   106,    98,    99,   100,   101,   102,     0,     0,
     105,    99,   100,   101,   102,   106,     0,   105,     0,     0,
       0,     0,   106
};

static const yytype_int16 yycheck[] =
{
      65,    66,    67,    68,    33,     0,     1,    36,     6,     6,
       6,    76,   143,   144,    26,     8,     5,    82,     1,   150,
       1,    19,    18,    18,     5,     6,    91,    92,    21,    25,
      25,    28,    97,    98,    99,   100,   101,   102,   103,   104,
      23,   106,    25,    26,    56,    12,    13,     1,    31,    16,
      79,     5,    35,     1,    21,     3,     1,    18,     3,     4,
       5,    31,     0,     1,    25,    35,    11,     1,     6,     3,
       4,     5,    17,    56,    19,     0,   141,    11,    23,    24,
      18,    26,    27,    17,    29,    19,     1,    25,     6,    23,
      23,     1,    26,    27,     1,    29,     6,    16,    19,     6,
      23,     6,    21,    18,     5,    20,     1,    23,    18,     1,
      25,     6,     7,    24,     6,    25,     8,     9,    10,    11,
      12,    13,    14,    15,    16,     1,    21,    20,     7,    21,
       6,    20,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    22,    20,     6,     1,    21,     3,     4,     5,     1,
      22,     3,     4,     5,    11,    21,     7,     6,    19,    11,
      17,    19,    19,    20,    19,    17,     1,    19,     3,     4,
       5,     1,    20,     3,     4,     5,    11,    24,     6,     6,
       5,    11,    17,    20,    19,    28,    20,    17,     1,    19,
       3,     4,     5,     1,    79,     3,     4,     5,    11,    22,
      29,    21,    73,    11,    17,   141,    19,    -1,    -1,    17,
       1,    19,     3,     4,     5,     1,    -1,     3,     4,     5,
      11,    -1,    -1,    -1,    -1,    11,    17,    -1,    19,    -1,
      -1,    17,     1,    19,     3,     4,     5,     1,    -1,     3,
       4,     5,    11,    -1,    -1,    -1,    -1,    11,    17,    -1,
      19,    -1,    -1,    17,     1,    19,     3,     4,     5,     1,
      -1,     3,     4,     5,    11,    -1,    -1,    -1,    -1,    11,
      17,    -1,    19,    -1,    -1,    17,     1,    19,     3,     4,
       5,     1,    -1,     3,     4,     5,    11,     3,     4,     5,
      -1,    11,    17,    -1,    19,    11,    -1,    17,    -1,    19,
      -1,    17,    -1,    19,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    22,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    20,    21,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      20,    21,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    20,    21,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
       9,    10,    11,    12,    13,    14,    -1,    16,    -1,    -1,
      -1,    -1,    21,     9,    10,    11,    12,    13,    -1,    -1,
      16,    10,    11,    12,    13,    21,    -1,    16,    -1,    -1,
      -1,    -1,    21
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    18,    25,    33,    34,    35,    37,    38,     6,
      37,     5,    39,    40,     0,    34,     1,     5,     6,    36,
      41,    42,     6,    23,     6,    19,    19,     6,     1,     7,
      21,    23,    45,    37,    48,    49,    37,    43,    44,     1,
      20,    43,    34,     5,    36,     1,     3,    48,     1,    41,
      50,    51,    24,    48,    41,    20,     7,    20,    20,    22,
      22,     1,     3,     4,     5,    11,    17,    19,    26,    27,
      29,    45,    46,    47,    52,     6,     8,     1,     6,     7,
      43,     6,    19,     1,    52,     1,    52,     1,    52,     1,
      52,    19,    19,    24,    46,     1,     6,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    21,    52,    50,     1,
      20,    52,    53,    20,    20,     6,     1,     6,    52,    52,
       6,     1,    52,     1,    52,     1,    52,     1,    52,     1,
      52,     1,    52,     1,    52,     1,    52,     5,     1,    52,
      20,     7,    20,    20,    20,    22,    22,    53,    47,    47,
      28,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    35,
      35,    36,    36,    36,    37,    37,    38,    38,    39,    39,
      40,    41,    41,    41,    42,    42,    42,    42,    43,    43,
      44,    45,    46,    46,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    48,    48,    49,    49,    49,    50,
      50,    51,    51,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     3,
       3,     1,     3,     3,     1,     1,     5,     2,     1,     0,
       1,     1,     4,     4,     4,     3,     4,     4,     3,     1,
       2,     4,     2,     0,     2,     1,     3,     5,     7,     5,
       2,     3,     3,     3,     2,     0,     3,     3,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     4,     3,     4,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     4,     4,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* Program: ExtDefList  */
#line 132 "./parser.y"
{
    // 创建Program节点，行号为外部定义列表的起始行
    (yyval.node) = create_ast_node("Program", (yyloc).first_line);
    // 为Program节点添加子节点（ExtDefList）
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
    // 设置AST根节点
    ast_root = (yyval.node);
}
#line 1873 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 143 "./parser.y"
{
    (yyval.node) = create_ast_node("ExtDefList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));  // 子节点：当前ExtDef + 剩余ExtDefList
}
#line 1882 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 148 "./parser.y"
{
    (yyval.node) = NULL;  // 空列表无需创建节点
    emptystart = yylineno;  // 记录空列表的起始行号
}
#line 1891 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 155 "./parser.y"
{
    // 形式1：类型说明符 + 外部声明列表 + 分号（如int a, b[10];）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1901 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 161 "./parser.y"
{
    // 形式2：类型说明符 + 分号（如struct S;  forward声明）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1911 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 167 "./parser.y"
{
    // 形式3：类型说明符 + 函数声明 + 复合语句（如int add(int a) { return a; }）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1921 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: error SEMI  */
#line 173 "./parser.y"
{
    // 错误处理1：任意错误 + 分号（如int a b;）
    syntaxError++;  // 错误计数+1
}
#line 1930 "./syntax.tab.c"
    break;

  case 9: /* ExtDef: Specifier error SEMI  */
#line 178 "./parser.y"
{
    // 错误处理2：类型说明符 + 错误 + 分号（如int ;）
    syntaxError++;
}
#line 1939 "./syntax.tab.c"
    break;

  case 10: /* ExtDef: error Specifier SEMI  */
#line 183 "./parser.y"
{
    // 错误处理3：错误 + 类型说明符 + 分号（如a int;）
    syntaxError++;
}
#line 1948 "./syntax.tab.c"
    break;

  case 11: /* ExtDecList: VarDec  */
#line 190 "./parser.y"
{
    // 单个变量声明（如a）
    (yyval.node) = create_ast_node("ExtDecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1958 "./syntax.tab.c"
    break;

  case 12: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 196 "./parser.y"
{
    // 多个变量声明（如a, b, c[5]）
    (yyval.node) = create_ast_node("ExtDecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1968 "./syntax.tab.c"
    break;

  case 13: /* ExtDecList: VarDec error ExtDefList  */
#line 202 "./parser.y"
{
    // 错误处理：变量声明 + 错误 + 外部定义列表（如a b int c;）
    syntaxError++;
}
#line 1977 "./syntax.tab.c"
    break;

  case 14: /* Specifier: TYPE  */
#line 211 "./parser.y"
{
    // 基础类型（如int、float）
    (yyval.node) = create_ast_node("Specifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1987 "./syntax.tab.c"
    break;

  case 15: /* Specifier: StructSpecifier  */
#line 217 "./parser.y"
{
    // 结构体类型（如struct S { int x; }）
    (yyval.node) = create_ast_node("Specifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1997 "./syntax.tab.c"
    break;

  case 16: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 225 "./parser.y"
{
    // 形式1：struct + 可选标签 + { 定义列表 }（如struct S { int x; }）
    (yyval.node) = create_ast_node("StructSpecifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2007 "./syntax.tab.c"
    break;

  case 17: /* StructSpecifier: STRUCT Tag  */
#line 231 "./parser.y"
{
    // 形式2：struct + 标签（引用已定义的结构体，如struct S）
    (yyval.node) = create_ast_node("StructSpecifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2017 "./syntax.tab.c"
    break;

  case 18: /* OptTag: ID  */
#line 239 "./parser.y"
{
    // 有标签（如struct S中的S）
    (yyval.node) = create_ast_node("OptTag", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2027 "./syntax.tab.c"
    break;

  case 19: /* OptTag: %empty  */
#line 245 "./parser.y"
{
    (yyval.node) = NULL;  // 空标签无需创建节点
}
#line 2035 "./syntax.tab.c"
    break;

  case 20: /* Tag: ID  */
#line 251 "./parser.y"
{
    (yyval.node) = create_ast_node("Tag", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2044 "./syntax.tab.c"
    break;

  case 21: /* VarDec: ID  */
#line 260 "./parser.y"
{
    // 普通变量（如a）
    (yyval.node) = create_ast_node("VarDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2054 "./syntax.tab.c"
    break;

  case 22: /* VarDec: VarDec LB INT RB  */
#line 266 "./parser.y"
{
    // 数组变量（如a[10]、b[2][3]）
    (yyval.node) = create_ast_node("VarDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2064 "./syntax.tab.c"
    break;

  case 23: /* VarDec: VarDec LB error RB  */
#line 272 "./parser.y"
{
    // 错误处理：数组下标错误（如a[b]中的b不是常量）
    syntaxError++;
}
#line 2073 "./syntax.tab.c"
    break;

  case 24: /* FunDec: ID LP VarList RP  */
#line 279 "./parser.y"
{
    // 带参数的函数（如add(int a, int b)）
    (yyval.node) = create_ast_node("FunDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2083 "./syntax.tab.c"
    break;

  case 25: /* FunDec: ID LP RP  */
#line 285 "./parser.y"
{
    // 无参数的函数（如foo()）
    (yyval.node) = create_ast_node("FunDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2093 "./syntax.tab.c"
    break;

  case 26: /* FunDec: ID LP error RP  */
#line 291 "./parser.y"
{
    // 错误处理：参数列表错误（如add(int a b)）
    syntaxError++;
}
#line 2102 "./syntax.tab.c"
    break;

  case 27: /* FunDec: error LP VarList RP  */
#line 296 "./parser.y"
{
    // 错误处理：函数名错误（如123(int a)）
    syntaxError++;
}
#line 2111 "./syntax.tab.c"
    break;

  case 28: /* VarList: ParamDec COMMA VarList  */
#line 303 "./parser.y"
{
    // 多个参数（如int a, float b）
    (yyval.node) = create_ast_node("VarList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2121 "./syntax.tab.c"
    break;

  case 29: /* VarList: ParamDec  */
#line 309 "./parser.y"
{
    // 单个参数（如int a）
    (yyval.node) = create_ast_node("VarList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2131 "./syntax.tab.c"
    break;

  case 30: /* ParamDec: Specifier VarDec  */
#line 317 "./parser.y"
{
    (yyval.node) = create_ast_node("ParamDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2140 "./syntax.tab.c"
    break;

  case 31: /* CompSt: LC DefList StmtList RC  */
#line 326 "./parser.y"
{
    (yyval.node) = create_ast_node("CompSt", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2149 "./syntax.tab.c"
    break;

  case 32: /* StmtList: Stmt StmtList  */
#line 333 "./parser.y"
{
    // 多个语句（如a=1; return a;）
    (yyval.node) = create_ast_node("StmtList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2159 "./syntax.tab.c"
    break;

  case 33: /* StmtList: %empty  */
#line 339 "./parser.y"
{
    (yyval.node) = NULL;  // 空语句列表无需创建节点
}
#line 2167 "./syntax.tab.c"
    break;

  case 34: /* Stmt: Exp SEMI  */
#line 346 "./parser.y"
{
    // 1. 表达式语句（如a = 1 + 2;、foo(3);）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2177 "./syntax.tab.c"
    break;

  case 35: /* Stmt: CompSt  */
#line 352 "./parser.y"
{
    // 2. 复合语句（嵌套的{...}块，如if/while的循环体）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2187 "./syntax.tab.c"
    break;

  case 36: /* Stmt: RETURN Exp SEMI  */
#line 358 "./parser.y"
{
    // 3. 返回语句（如return a;、return 0;）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2197 "./syntax.tab.c"
    break;

  case 37: /* Stmt: IF LP Exp RP Stmt  */
#line 364 "./parser.y"
{
    // 4. 无else的if语句（如if(a > 0) { ... }）
    // %prec LOWER_THAN_ELSE：强制if优先级低于else，避免else匹配歧义
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2208 "./syntax.tab.c"
    break;

  case 38: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 371 "./parser.y"
{
    // 5. 带else的if语句（如if(a > 0) { ... } else { ... }）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 7, (yyvsp[-6].node), (yyvsp[-5].node), (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2218 "./syntax.tab.c"
    break;

  case 39: /* Stmt: WHILE LP Exp RP Stmt  */
#line 377 "./parser.y"
{
    // 6. while循环语句（如while(a > 0) { ... }）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2228 "./syntax.tab.c"
    break;

  case 40: /* Stmt: error SEMI  */
#line 383 "./parser.y"
{
    // 错误处理1：任意错误+分号（如a b;、123;）
    syntaxError++;
}
#line 2237 "./syntax.tab.c"
    break;

  case 41: /* Stmt: Exp error SEMI  */
#line 388 "./parser.y"
{
    // 错误处理2：表达式不完整（如a + ;、a = ;）
    syntaxError++;
}
#line 2246 "./syntax.tab.c"
    break;

  case 42: /* Stmt: RETURN Exp error  */
#line 393 "./parser.y"
{
    // 错误处理3：return语句缺少分号（如return a）
    syntaxError++;
}
#line 2255 "./syntax.tab.c"
    break;

  case 43: /* Stmt: RETURN error SEMI  */
#line 398 "./parser.y"
{
    // 错误处理4：return后无表达式（如return ;）
    syntaxError++;
}
#line 2264 "./syntax.tab.c"
    break;

  case 44: /* DefList: Def DefList  */
#line 407 "./parser.y"
{
    (yyval.node) = create_ast_node("DefList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2273 "./syntax.tab.c"
    break;

  case 45: /* DefList: %empty  */
#line 412 "./parser.y"
{
    (yyval.node) = NULL;
}
#line 2281 "./syntax.tab.c"
    break;

  case 46: /* Def: Specifier DecList SEMI  */
#line 418 "./parser.y"
{
    (yyval.node) = create_ast_node("Def", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2290 "./syntax.tab.c"
    break;

  case 47: /* Def: Specifier error SEMI  */
#line 423 "./parser.y"
{
    // 错误处理1：类型后声明错误（如int ;、int a b;）
    syntaxError++;
}
#line 2299 "./syntax.tab.c"
    break;

  case 48: /* Def: Specifier DecList error  */
#line 428 "./parser.y"
{
    // 错误处理2：定义缺少分号（如int a = 1）
    syntaxError++;
}
#line 2308 "./syntax.tab.c"
    break;

  case 49: /* DecList: Dec  */
#line 435 "./parser.y"
{
    (yyval.node) = create_ast_node("DecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2317 "./syntax.tab.c"
    break;

  case 50: /* DecList: Dec COMMA DecList  */
#line 440 "./parser.y"
{
    // 多个声明（如a = 1, b[5] = {0}, c）
    (yyval.node) = create_ast_node("DecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2327 "./syntax.tab.c"
    break;

  case 51: /* Dec: VarDec  */
#line 448 "./parser.y"
{
    // 无初始化的声明（如int a;、float b[10];）
    (yyval.node) = create_ast_node("Dec", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2337 "./syntax.tab.c"
    break;

  case 52: /* Dec: VarDec ASSIGNOP Exp  */
#line 454 "./parser.y"
{
    // 带初始化的声明（如int a = 1 + 2;、float b = 3.14;）
    (yyval.node) = create_ast_node("Dec", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2347 "./syntax.tab.c"
    break;

  case 53: /* Exp: Exp ASSIGNOP Exp  */
#line 464 "./parser.y"
{
    // 1. 赋值表达式（如a = b、a = b = c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2357 "./syntax.tab.c"
    break;

  case 54: /* Exp: Exp AND Exp  */
#line 470 "./parser.y"
{
    // 2. 逻辑与表达式（如a > 0 && b < 10）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2367 "./syntax.tab.c"
    break;

  case 55: /* Exp: Exp OR Exp  */
#line 476 "./parser.y"
{
    // 3. 逻辑或表达式（如a == 0 || b == 1）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2377 "./syntax.tab.c"
    break;

  case 56: /* Exp: Exp RELOP Exp  */
#line 482 "./parser.y"
{
    // 4. 关系表达式（如a > 5、b <= 3、c == 0）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2387 "./syntax.tab.c"
    break;

  case 57: /* Exp: Exp PLUS Exp  */
#line 488 "./parser.y"
{
    // 5. 加法表达式（如a + 3、b + c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2397 "./syntax.tab.c"
    break;

  case 58: /* Exp: Exp MINUS Exp  */
#line 494 "./parser.y"
{
    // 6. 减法表达式（如a - 5、b - c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2407 "./syntax.tab.c"
    break;

  case 59: /* Exp: Exp STAR Exp  */
#line 500 "./parser.y"
{
    // 7. 乘法表达式（如a * 4、b * c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2417 "./syntax.tab.c"
    break;

  case 60: /* Exp: Exp DIV Exp  */
#line 506 "./parser.y"
{
    // 8. 除法表达式（如a / 2、b / c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2427 "./syntax.tab.c"
    break;

  case 61: /* Exp: LP Exp RP  */
#line 512 "./parser.y"
{
    // 9. 括号表达式（强制改变优先级，如(a + b) * c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2437 "./syntax.tab.c"
    break;

  case 62: /* Exp: MINUS Exp  */
#line 518 "./parser.y"
{
    // 10. 负号表达式（如-3、-a，%prec UMINUS指定为单目运算符）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2447 "./syntax.tab.c"
    break;

  case 63: /* Exp: NOT Exp  */
#line 524 "./parser.y"
{
    // 11. 逻辑非表达式（如!a、!(b > 0)）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2457 "./syntax.tab.c"
    break;

  case 64: /* Exp: ID LP Args RP  */
#line 530 "./parser.y"
{
    // 12. 带参数的函数调用（如add(a, b)、foo(1 + 2)）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2467 "./syntax.tab.c"
    break;

  case 65: /* Exp: ID LP RP  */
#line 536 "./parser.y"
{
    // 13. 无参数的函数调用（如foo()、init()）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2477 "./syntax.tab.c"
    break;

  case 66: /* Exp: Exp LB Exp RB  */
#line 542 "./parser.y"
{
    // 14. 数组下标访问（如a[5]、b[i + 1]）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2487 "./syntax.tab.c"
    break;

  case 67: /* Exp: Exp DOT ID  */
#line 548 "./parser.y"
{
    // 15. 结构体成员访问（如s.x、student.age）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2497 "./syntax.tab.c"
    break;

  case 68: /* Exp: ID  */
#line 554 "./parser.y"
{
    // 16. 标识符表达式（变量名，如a、b）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2507 "./syntax.tab.c"
    break;

  case 69: /* Exp: INT  */
#line 560 "./parser.y"
{
    // 17. 整型常量表达式（如123、0、-456）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2517 "./syntax.tab.c"
    break;

  case 70: /* Exp: FLOAT  */
#line 566 "./parser.y"
{
    // 18. 浮点型常量表达式（如3.14、0.5、-2.718）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2527 "./syntax.tab.c"
    break;

  case 71: /* Exp: Exp ASSIGNOP error  */
#line 573 "./parser.y"
{
    // 错误1：赋值表达式右侧不完整（如a = ）
    syntaxError++;
}
#line 2536 "./syntax.tab.c"
    break;

  case 72: /* Exp: Exp AND error  */
#line 578 "./parser.y"
{
    // 错误2：逻辑与右侧不完整（如a && ）
    syntaxError++;
}
#line 2545 "./syntax.tab.c"
    break;

  case 73: /* Exp: Exp OR error  */
#line 583 "./parser.y"
{
    // 错误3：逻辑或右侧不完整（如a || ）
    syntaxError++;
}
#line 2554 "./syntax.tab.c"
    break;

  case 74: /* Exp: Exp RELOP error  */
#line 588 "./parser.y"
{
    // 错误4：关系表达式右侧不完整（如a > ）
    syntaxError++;
}
#line 2563 "./syntax.tab.c"
    break;

  case 75: /* Exp: Exp PLUS error  */
#line 593 "./parser.y"
{
    // 错误5：加法表达式右侧不完整（如a + ）
    syntaxError++;
}
#line 2572 "./syntax.tab.c"
    break;

  case 76: /* Exp: Exp MINUS error  */
#line 598 "./parser.y"
{
    // 错误6：减法表达式右侧不完整（如a - ）
    syntaxError++;
}
#line 2581 "./syntax.tab.c"
    break;

  case 77: /* Exp: Exp STAR error  */
#line 603 "./parser.y"
{
    // 错误7：乘法表达式右侧不完整（如a * ）
    syntaxError++;
}
#line 2590 "./syntax.tab.c"
    break;

  case 78: /* Exp: Exp DIV error  */
#line 608 "./parser.y"
{
    // 错误8：除法表达式右侧不完整（如a / ）
    syntaxError++;
}
#line 2599 "./syntax.tab.c"
    break;

  case 79: /* Exp: LP error RP  */
#line 613 "./parser.y"
{
    // 错误9：括号内表达式错误（如(a b)）
    syntaxError++;
}
#line 2608 "./syntax.tab.c"
    break;

  case 80: /* Exp: MINUS error  */
#line 618 "./parser.y"
{
    // 错误10：负号后无表达式（如- ）
    syntaxError++;
}
#line 2617 "./syntax.tab.c"
    break;

  case 81: /* Exp: NOT error  */
#line 623 "./parser.y"
{
    // 错误11：逻辑非后无表达式（如! ）
    syntaxError++;
}
#line 2626 "./syntax.tab.c"
    break;

  case 82: /* Exp: ID LP error RP  */
#line 628 "./parser.y"
{
    // 错误12：函数参数列表错误（如foo(a b)）
    syntaxError++;
}
#line 2635 "./syntax.tab.c"
    break;

  case 83: /* Exp: Exp LB error RB  */
#line 633 "./parser.y"
{
    // 错误13：数组下标错误（如a[ ]、a[b c]）
    syntaxError++;
}
#line 2644 "./syntax.tab.c"
    break;

  case 84: /* Args: Exp COMMA Args  */
#line 640 "./parser.y"
{
    // 多个实参（如add(a + b, 3, c)）
    (yyval.node) = create_ast_node("Args", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2654 "./syntax.tab.c"
    break;

  case 85: /* Args: Exp  */
#line 646 "./parser.y"
{
    // 单个实参（如foo(a)、bar(1 + 2)）
    (yyval.node) = create_ast_node("Args", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2664 "./syntax.tab.c"
    break;


#line 2668 "./syntax.tab.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 652 "./parser.y"


// -------------------------- 辅助函数实现 --------------------------
// 1. 语法错误处理函数（Bison默认调用yyerror）
int yyerror(const char *msg) {
    syntaxError++;
    // 输出错误行号和提示（msg为Bison生成的错误描述）
    fprintf(stderr, "Error type B at Line %d: %s.\n", yylineno, msg);
    return 0;
}

// 2. 创建AST节点：初始化节点名称、行号，清空子节点/兄弟节点指针

ASTNode *create_ast_node(const char *node_name, int line_num) {
    ASTNode *new_node = malloc(sizeof(ASTNode));
    new_node->name = strdup(node_name);  // 注意：const char*，不能 strncpy
    new_node->line = line_num;
    new_node->kind = ASTK_NONTERM;
    new_node->nchild = 0;
    new_node->child = NULL;
    return new_node;
}

// 3. 为父节点添加子节点：支持可变参数（多个子节点）
void add_child_nodes(ASTNode *parent, int arg_count, ...) {
    if (!parent || arg_count <= 0) return;

    parent->nchild = arg_count;
    parent->child = malloc(sizeof(ASTNode*) * arg_count);

    va_list args;
    va_start(args, arg_count);
    for (int i = 0; i < arg_count; i++) {
        parent->child[i] = va_arg(args, ASTNode*);
    }
    va_end(args);
}

// 4. 遍历并打印AST（递归实现，按缩进显示层级关系）
void traverse_ast(const ASTNode *curr_node, int depth) {
    if (curr_node == NULL) return;          /* 递归终止 */

    /* 1. 打印缩进 */
    for (int i = 0; i < depth; ++i) printf("  ");

    /* 2. 打印节点名 */
    printf("%s", curr_node->name);

    /* 3. 打印附加信息（仅对叶子或特定 kind） */
    switch (curr_node->kind) {
        case ASTK_INT:
            printf(": %lld", curr_node->ival);
            break;
        case ASTK_FLOAT:
            printf(": %.6f", curr_node->fval);   /* 保留 6 位小数，与原代码一致 */
            break;
        case ASTK_ID:
        case ASTK_TYPE:
            printf(": %s", curr_node->sval);
            break;
        default:
            /* 非终结符或其他情况，只打印行号（当 line > 0） */
            if (curr_node->line > 0)
                printf(" (Line %d)", curr_node->line);
            break;
    }
    printf("\n");

    /* 4. 递归访问子结点 */
    for (int i = 0; i < curr_node->nchild; ++i)
        traverse_ast(curr_node->child[i], depth + 1);
}
