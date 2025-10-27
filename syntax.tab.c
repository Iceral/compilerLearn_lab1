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
#line 8 "./parser.y"

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

// 词法分析器入口函数（从lex.yy.c导入）
extern int yylex(void);

#line 101 "./syntax.tab.c"

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

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

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
#endif /* !defined yyoverflow */

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
#define YYLAST   401

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  150

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
       0,   128,   128,   139,   145,   151,   157,   163,   169,   175,
     181,   189,   195,   206,   212,   220,   226,   234,   241,   246,
     255,   261,   267,   274,   280,   286,   291,   298,   304,   312,
     321,   328,   335,   341,   348,   354,   360,   367,   373,   379,
     384,   389,   394,   403,   409,   414,   419,   424,   431,   436,
     444,   450,   460,   466,   472,   478,   484,   490,   496,   502,
     508,   514,   520,   526,   532,   538,   544,   550,   556,   562,
     569,   574,   579,   584,   589,   594,   599,   604,   609,   614,
     619,   624,   629,   636,   642
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT", "ID",
  "SEMI", "COMMA", "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV",
  "AND", "OR", "DOT", "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC",
  "STRUCT", "RETURN", "IF", "ELSE", "WHILE", "UMINUS", "LOWER_THAN_ELSE",
  "$accept", "Program", "ExtDefList", "ExtDef", "ExtDecList", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "VarDec", "FunDec", "VarList",
  "ParamDec", "CompSt", "StmtList", "Stmt", "DefList", "Def", "DecList",
  "Dec", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-129)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-33)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      24,    38,  -129,     3,    19,  -129,    24,   374,  -129,  -129,
      58,    60,    61,  -129,  -129,  -129,     9,    62,  -129,    83,
       0,    67,  -129,    -2,  -129,    -2,    30,  -129,    92,    59,
      -2,  -129,    17,    76,    -2,    92,    91,   106,   107,  -129,
     108,  -129,  -129,   104,   113,    42,   134,    46,    51,   135,
    -129,  -129,   120,  -129,    -2,  -129,  -129,  -129,  -129,   137,
    -129,  -129,   127,    74,   133,   150,   155,   130,   138,  -129,
     139,    42,    93,  -129,   263,  -129,  -129,    92,  -129,  -129,
     128,  -129,    66,  -129,    66,   142,   305,   158,   109,   263,
     263,  -129,  -129,   159,  -129,   172,   177,   194,   199,   216,
     221,   238,   243,   163,   260,   347,  -129,   151,  -129,   276,
     164,  -129,  -129,  -129,  -129,  -129,   319,   333,  -129,  -129,
     347,  -129,   380,  -129,   257,  -129,   257,  -129,    66,  -129,
      66,  -129,   373,  -129,   360,  -129,   148,   290,  -129,   263,
    -129,    69,    69,  -129,  -129,  -129,   157,  -129,    69,  -129
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    13,    18,     0,     2,     0,     0,    14,     8,
       0,    19,     0,    16,     1,     3,     0,    20,     6,     0,
      11,     0,    10,    44,     9,     0,     0,     5,     0,     0,
      44,     7,     0,     0,    44,     0,     0,    28,     0,    24,
       0,    20,    12,     0,     0,     0,     0,    50,     0,    48,
      15,    43,    29,    26,     0,    25,    23,    22,    21,     0,
      68,    69,    67,     0,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    46,     0,    47,    45,     0,    27,    39,
       0,    79,    61,    80,    62,     0,     0,     0,     0,     0,
       0,    30,    31,     0,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    49,     0,    64,    84,
       0,    78,    60,    42,    41,    35,     0,     0,    40,    70,
      52,    73,    55,    74,    56,    75,    57,    76,    58,    77,
      59,    71,    53,    72,    54,    66,     0,     0,    81,     0,
      63,     0,     0,    82,    65,    83,    36,    38,     0,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -129,  -129,   173,  -129,   162,     4,  -129,  -129,  -129,   -26,
    -129,   -14,  -129,   165,   116,  -128,    82,  -129,   115,  -129,
     -63,    54
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    19,    32,     8,    12,    13,    20,
      21,    36,    37,    69,    70,    71,    33,    34,    48,    49,
      72,   110
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,    84,    86,    88,     7,    10,    47,    28,    11,    52,
       7,   105,    40,   146,   147,    24,     2,   109,    46,    14,
     149,    29,    41,     3,    -4,     1,   116,   117,    25,    35,
      35,    38,   120,   122,   124,   126,   128,   130,   132,   134,
      78,   137,     2,    59,     9,    60,    61,    62,     2,     3,
      39,    47,    75,    63,    74,     3,     2,    76,    35,    64,
      43,    65,    44,     3,    22,    30,   -32,    29,    66,    67,
      59,    68,    60,    61,    62,    81,   109,    60,    61,    62,
      63,    26,   103,   -17,    23,    63,    64,   104,    65,    27,
      30,    64,    30,    65,    93,    66,    67,    41,    68,    94,
      50,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     114,    53,    45,    54,   104,   115,    51,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    57,    55,    56,   107,
     104,    60,    61,    62,    83,    58,    60,    61,    62,    63,
      73,    29,    77,    79,    63,    64,    80,    65,   108,    89,
      64,    85,    65,    60,    61,    62,    87,    90,    60,    61,
      62,    63,   111,    91,   113,   118,    63,    64,   135,    65,
     143,   138,    64,   119,    65,    60,    61,    62,   121,    15,
      60,    61,    62,    63,   140,   148,    31,    92,    63,    64,
      42,    65,   106,   145,    64,   123,    65,    60,    61,    62,
     125,     0,    60,    61,    62,    63,     0,     0,     0,     0,
      63,    64,     0,    65,     0,     0,    64,   127,    65,    60,
      61,    62,   129,     0,    60,    61,    62,    63,     0,     0,
       0,     0,    63,    64,     0,    65,     0,     0,    64,   131,
      65,    60,    61,    62,   133,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    63,    64,     0,    65,     0,     0,
      64,   136,    65,    60,    61,    62,    60,    61,    62,    99,
     100,    63,     0,   103,    63,     0,     0,    64,   104,    65,
      64,     0,    65,   139,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,     0,     0,     0,   104,    95,    96,
      97,    98,    99,   100,   101,   102,   103,     0,     0,     0,
       0,   104,   144,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,     0,     0,   112,   104,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,     0,     0,   141,
     104,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,     0,     0,   142,   104,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,     0,     0,     0,   104,    96,
      97,    98,    99,   100,   101,    16,   103,     0,     0,    17,
      18,   104,    96,    97,    98,    99,   100,     0,     0,   103,
      97,    98,    99,   100,   104,     0,   103,     0,     0,     0,
       0,   104
};

static const yytype_int16 yycheck[] =
{
      63,    64,    65,    66,     0,     1,    32,     7,     5,    35,
       6,    74,    26,   141,   142,     6,    18,    80,     1,     0,
     148,    21,     5,    25,     0,     1,    89,    90,    19,    25,
      26,     1,    95,    96,    97,    98,    99,   100,   101,   102,
      54,   104,    18,     1,     6,     3,     4,     5,    18,    25,
      20,    77,     1,    11,     8,    25,    18,     6,    54,    17,
       1,    19,     3,    25,     6,    23,    24,    21,    26,    27,
       1,    29,     3,     4,     5,     1,   139,     3,     4,     5,
      11,    19,    16,    23,    23,    11,    17,    21,    19,     6,
      23,    17,    23,    19,     1,    26,    27,     5,    29,     6,
      24,     8,     9,    10,    11,    12,    13,    14,    15,    16,
       1,    20,    30,     7,    21,     6,    34,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    22,    20,    20,     1,
      21,     3,     4,     5,     1,    22,     3,     4,     5,    11,
       6,    21,     7,     6,    11,    17,    19,    19,    20,    19,
      17,     1,    19,     3,     4,     5,     1,    19,     3,     4,
       5,    11,    20,    24,     6,     6,    11,    17,     5,    19,
      22,    20,    17,     1,    19,     3,     4,     5,     1,     6,
       3,     4,     5,    11,    20,    28,    21,    71,    11,    17,
      28,    19,    77,   139,    17,     1,    19,     3,     4,     5,
       1,    -1,     3,     4,     5,    11,    -1,    -1,    -1,    -1,
      11,    17,    -1,    19,    -1,    -1,    17,     1,    19,     3,
       4,     5,     1,    -1,     3,     4,     5,    11,    -1,    -1,
      -1,    -1,    11,    17,    -1,    19,    -1,    -1,    17,     1,
      19,     3,     4,     5,     1,    -1,     3,     4,     5,    11,
      -1,    -1,    -1,    -1,    11,    17,    -1,    19,    -1,    -1,
      17,     1,    19,     3,     4,     5,     3,     4,     5,    12,
      13,    11,    -1,    16,    11,    -1,    -1,    17,    21,    19,
      17,    -1,    19,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    21,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    21,    22,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    20,    21,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    20,    21,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,     9,
      10,    11,    12,    13,    14,     1,    16,    -1,    -1,     5,
       6,    21,     9,    10,    11,    12,    13,    -1,    -1,    16,
      10,    11,    12,    13,    21,    -1,    16,    -1,    -1,    -1,
      -1,    21
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    18,    25,    33,    34,    35,    37,    38,     6,
      37,     5,    39,    40,     0,    34,     1,     5,     6,    36,
      41,    42,     6,    23,     6,    19,    19,     6,     7,    21,
      23,    45,    37,    48,    49,    37,    43,    44,     1,    20,
      43,     5,    36,     1,     3,    48,     1,    41,    50,    51,
      24,    48,    41,    20,     7,    20,    20,    22,    22,     1,
       3,     4,     5,    11,    17,    19,    26,    27,    29,    45,
      46,    47,    52,     6,     8,     1,     6,     7,    43,     6,
      19,     1,    52,     1,    52,     1,    52,     1,    52,    19,
      19,    24,    46,     1,     6,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    21,    52,    50,     1,    20,    52,
      53,    20,    20,     6,     1,     6,    52,    52,     6,     1,
      52,     1,    52,     1,    52,     1,    52,     1,    52,     1,
      52,     1,    52,     1,    52,     5,     1,    52,    20,     7,
      20,    20,    20,    22,    22,    53,    47,    47,    28,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    35,
      35,    36,    36,    37,    37,    38,    38,    39,    39,    40,
      41,    41,    41,    42,    42,    42,    42,    43,    43,    44,
      45,    46,    46,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    48,    48,    49,    49,    49,    50,    50,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     3,
       3,     1,     3,     1,     1,     5,     2,     1,     0,     1,
       1,     4,     4,     4,     3,     4,     4,     3,     1,     2,
       4,     2,     0,     2,     1,     3,     5,     7,     5,     2,
       3,     3,     3,     2,     0,     3,     3,     3,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     4,     3,     4,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     4,     4,     3,     1
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
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
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
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 129 "./parser.y"
{
    // 创建Program节点，行号为外部定义列表的起始行
    (yyval.node) = create_ast_node("Program", (yyloc).first_line);
    // 为Program节点添加子节点（ExtDefList）
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
    // 设置AST根节点
    ast_root = (yyval.node);
}
#line 1409 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 140 "./parser.y"
{
    (yyval.node) = create_ast_node("ExtDefList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));  // 子节点：当前ExtDef + 剩余ExtDefList
}
#line 1418 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 145 "./parser.y"
{
    (yyval.node) = NULL;  // 空列表无需创建节点
    emptystart = yylineno;  // 记录空列表的起始行号
}
#line 1427 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 152 "./parser.y"
{
    // 形式1：类型说明符 + 外部声明列表 + 分号（如int a, b[10];）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1437 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 158 "./parser.y"
{
    // 形式2：类型说明符 + 分号（如struct S;  forward声明）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1447 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 164 "./parser.y"
{
    // 形式3：类型说明符 + 函数声明 + 复合语句（如int add(int a) { return a; }）
    (yyval.node) = create_ast_node("ExtDef", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1457 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: error SEMI  */
#line 170 "./parser.y"
{
    // 错误处理1：任意错误 + 分号（如int a b;）
    syntaxError++;  // 错误计数+1
    yyerrok;
}
#line 1467 "./syntax.tab.c"
    break;

  case 9: /* ExtDef: Specifier error SEMI  */
#line 176 "./parser.y"
{
    // 错误处理2：类型说明符 + 错误 + 分号（如int ;）
    syntaxError++;
    yyerrok;
}
#line 1477 "./syntax.tab.c"
    break;

  case 10: /* ExtDef: error Specifier SEMI  */
#line 182 "./parser.y"
{
    // 错误处理3：错误 + 类型说明符 + 分号（如a int;）
    syntaxError++;
    yyerrok;
}
#line 1487 "./syntax.tab.c"
    break;

  case 11: /* ExtDecList: VarDec  */
#line 190 "./parser.y"
{
    // 单个变量声明（如a）
    (yyval.node) = create_ast_node("ExtDecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1497 "./syntax.tab.c"
    break;

  case 12: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 196 "./parser.y"
{
    // 多个变量声明（如a, b, c[5]）
    (yyval.node) = create_ast_node("ExtDecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1507 "./syntax.tab.c"
    break;

  case 13: /* Specifier: TYPE  */
#line 207 "./parser.y"
{
    // 基础类型（如int、float）
    (yyval.node) = create_ast_node("Specifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1517 "./syntax.tab.c"
    break;

  case 14: /* Specifier: StructSpecifier  */
#line 213 "./parser.y"
{
    // 结构体类型（如struct S { int x; }）
    (yyval.node) = create_ast_node("Specifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1527 "./syntax.tab.c"
    break;

  case 15: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 221 "./parser.y"
{
    // 形式1：struct + 可选标签 + { 定义列表 }（如struct S { int x; }）
    (yyval.node) = create_ast_node("StructSpecifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1537 "./syntax.tab.c"
    break;

  case 16: /* StructSpecifier: STRUCT Tag  */
#line 227 "./parser.y"
{
    // 形式2：struct + 标签（引用已定义的结构体，如struct S）
    (yyval.node) = create_ast_node("StructSpecifier", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1547 "./syntax.tab.c"
    break;

  case 17: /* OptTag: ID  */
#line 235 "./parser.y"
{
    // 有标签（如struct S中的S）
    (yyval.node) = create_ast_node("OptTag", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1557 "./syntax.tab.c"
    break;

  case 18: /* OptTag: %empty  */
#line 241 "./parser.y"
{
    (yyval.node) = NULL;  // 空标签无需创建节点
}
#line 1565 "./syntax.tab.c"
    break;

  case 19: /* Tag: ID  */
#line 247 "./parser.y"
{
    (yyval.node) = create_ast_node("Tag", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1574 "./syntax.tab.c"
    break;

  case 20: /* VarDec: ID  */
#line 256 "./parser.y"
{
    // 普通变量（如a）
    (yyval.node) = create_ast_node("VarDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1584 "./syntax.tab.c"
    break;

  case 21: /* VarDec: VarDec LB INT RB  */
#line 262 "./parser.y"
{
    // 数组变量（如a[10]、b[2][3]）
    (yyval.node) = create_ast_node("VarDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1594 "./syntax.tab.c"
    break;

  case 22: /* VarDec: VarDec LB error RB  */
#line 268 "./parser.y"
{
    // 错误处理：数组下标错误（如a[b]中的b不是常量）
    syntaxError++;
}
#line 1603 "./syntax.tab.c"
    break;

  case 23: /* FunDec: ID LP VarList RP  */
#line 275 "./parser.y"
{
    // 带参数的函数（如add(int a, int b)）
    (yyval.node) = create_ast_node("FunDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1613 "./syntax.tab.c"
    break;

  case 24: /* FunDec: ID LP RP  */
#line 281 "./parser.y"
{
    // 无参数的函数（如foo()）
    (yyval.node) = create_ast_node("FunDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1623 "./syntax.tab.c"
    break;

  case 25: /* FunDec: ID LP error RP  */
#line 287 "./parser.y"
{
    // 错误处理：参数列表错误（如add(int a b)）
    syntaxError++;
}
#line 1632 "./syntax.tab.c"
    break;

  case 26: /* FunDec: error LP VarList RP  */
#line 292 "./parser.y"
{
    // 错误处理：函数名错误（如123(int a)）
    syntaxError++;
}
#line 1641 "./syntax.tab.c"
    break;

  case 27: /* VarList: ParamDec COMMA VarList  */
#line 299 "./parser.y"
{
    // 多个参数（如int a, float b）
    (yyval.node) = create_ast_node("VarList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1651 "./syntax.tab.c"
    break;

  case 28: /* VarList: ParamDec  */
#line 305 "./parser.y"
{
    // 单个参数（如int a）
    (yyval.node) = create_ast_node("VarList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1661 "./syntax.tab.c"
    break;

  case 29: /* ParamDec: Specifier VarDec  */
#line 313 "./parser.y"
{
    (yyval.node) = create_ast_node("ParamDec", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1670 "./syntax.tab.c"
    break;

  case 30: /* CompSt: LC DefList StmtList RC  */
#line 322 "./parser.y"
{
    (yyval.node) = create_ast_node("CompSt", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1679 "./syntax.tab.c"
    break;

  case 31: /* StmtList: Stmt StmtList  */
#line 329 "./parser.y"
{
    // 多个语句（如a=1; return a;）
    (yyval.node) = create_ast_node("StmtList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1689 "./syntax.tab.c"
    break;

  case 32: /* StmtList: %empty  */
#line 335 "./parser.y"
{
    (yyval.node) = NULL;  // 空语句列表无需创建节点
}
#line 1697 "./syntax.tab.c"
    break;

  case 33: /* Stmt: Exp SEMI  */
#line 342 "./parser.y"
{
    // 1. 表达式语句（如a = 1 + 2;、foo(3);）
    // 关键：用SEMI的行号作为Stmt的行号
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1708 "./syntax.tab.c"
    break;

  case 34: /* Stmt: CompSt  */
#line 349 "./parser.y"
{
    // 2. 复合语句（嵌套的{...}块，如if/while的循环体）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1718 "./syntax.tab.c"
    break;

  case 35: /* Stmt: RETURN Exp SEMI  */
#line 355 "./parser.y"
{
    // 3. 返回语句（如return a;、return 0;）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1728 "./syntax.tab.c"
    break;

  case 36: /* Stmt: IF LP Exp RP Stmt  */
#line 361 "./parser.y"
{
    // 4. 无else的if语句（如if(a > 0) { ... }）
    // %prec LOWER_THAN_ELSE：强制if优先级低于else，避免else匹配歧义
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1739 "./syntax.tab.c"
    break;

  case 37: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 368 "./parser.y"
{
    // 5. 带else的if语句（如if(a > 0) { ... } else { ... }）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 7, (yyvsp[-6].node), (yyvsp[-5].node), (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1749 "./syntax.tab.c"
    break;

  case 38: /* Stmt: WHILE LP Exp RP Stmt  */
#line 374 "./parser.y"
{
    // 6. while循环语句（如while(a > 0) { ... }）
    (yyval.node) = create_ast_node("Stmt", (yyloc).first_line);
    add_child_nodes((yyval.node), 5, (yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1759 "./syntax.tab.c"
    break;

  case 39: /* Stmt: error SEMI  */
#line 380 "./parser.y"
{
    // 错误处理1：任意错误+分号（如a b;、123;）
    syntaxError++;
}
#line 1768 "./syntax.tab.c"
    break;

  case 40: /* Stmt: Exp error SEMI  */
#line 385 "./parser.y"
{
    // 错误处理2：表达式不完整（如a + ;、a = ;）
    syntaxError++;
}
#line 1777 "./syntax.tab.c"
    break;

  case 41: /* Stmt: RETURN Exp error  */
#line 390 "./parser.y"
{
    // 错误处理3：return语句缺少分号（如return a）
    syntaxError++;
}
#line 1786 "./syntax.tab.c"
    break;

  case 42: /* Stmt: RETURN error SEMI  */
#line 395 "./parser.y"
{
    // 错误处理4：return后无表达式（如return ;）
    syntaxError++;
}
#line 1795 "./syntax.tab.c"
    break;

  case 43: /* DefList: Def DefList  */
#line 404 "./parser.y"
{
    (yyval.node) = create_ast_node("DefList", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1804 "./syntax.tab.c"
    break;

  case 44: /* DefList: %empty  */
#line 409 "./parser.y"
{
    (yyval.node) = NULL;
}
#line 1812 "./syntax.tab.c"
    break;

  case 45: /* Def: Specifier DecList SEMI  */
#line 415 "./parser.y"
{
    (yyval.node) = create_ast_node("Def", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1821 "./syntax.tab.c"
    break;

  case 46: /* Def: Specifier error SEMI  */
#line 420 "./parser.y"
{
    // 错误处理1：类型后声明错误（如int ;、int a b;）
    syntaxError++;
}
#line 1830 "./syntax.tab.c"
    break;

  case 47: /* Def: Specifier DecList error  */
#line 425 "./parser.y"
{
    // 错误处理2：定义缺少分号（如int a = 1）
    syntaxError++;
}
#line 1839 "./syntax.tab.c"
    break;

  case 48: /* DecList: Dec  */
#line 432 "./parser.y"
{
    (yyval.node) = create_ast_node("DecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1848 "./syntax.tab.c"
    break;

  case 49: /* DecList: Dec COMMA DecList  */
#line 437 "./parser.y"
{
    // 多个声明（如a = 1, b[5] = {0}, c）
    (yyval.node) = create_ast_node("DecList", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1858 "./syntax.tab.c"
    break;

  case 50: /* Dec: VarDec  */
#line 445 "./parser.y"
{
    // 无初始化的声明（如int a;、float b[10];）
    (yyval.node) = create_ast_node("Dec", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 1868 "./syntax.tab.c"
    break;

  case 51: /* Dec: VarDec ASSIGNOP Exp  */
#line 451 "./parser.y"
{
    // 带初始化的声明（如int a = 1 + 2;、float b = 3.14;）
    (yyval.node) = create_ast_node("Dec", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1878 "./syntax.tab.c"
    break;

  case 52: /* Exp: Exp ASSIGNOP Exp  */
#line 461 "./parser.y"
{
    // 1. 赋值表达式（如a = b、a = b = c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1888 "./syntax.tab.c"
    break;

  case 53: /* Exp: Exp AND Exp  */
#line 467 "./parser.y"
{
    // 2. 逻辑与表达式（如a > 0 && b < 10）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1898 "./syntax.tab.c"
    break;

  case 54: /* Exp: Exp OR Exp  */
#line 473 "./parser.y"
{
    // 3. 逻辑或表达式（如a == 0 || b == 1）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1908 "./syntax.tab.c"
    break;

  case 55: /* Exp: Exp RELOP Exp  */
#line 479 "./parser.y"
{
    // 4. 关系表达式（如a > 5、b <= 3、c == 0）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1918 "./syntax.tab.c"
    break;

  case 56: /* Exp: Exp PLUS Exp  */
#line 485 "./parser.y"
{
    // 5. 加法表达式（如a + 3、b + c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1928 "./syntax.tab.c"
    break;

  case 57: /* Exp: Exp MINUS Exp  */
#line 491 "./parser.y"
{
    // 6. 减法表达式（如a - 5、b - c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1938 "./syntax.tab.c"
    break;

  case 58: /* Exp: Exp STAR Exp  */
#line 497 "./parser.y"
{
    // 7. 乘法表达式（如a * 4、b * c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1948 "./syntax.tab.c"
    break;

  case 59: /* Exp: Exp DIV Exp  */
#line 503 "./parser.y"
{
    // 8. 除法表达式（如a / 2、b / c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1958 "./syntax.tab.c"
    break;

  case 60: /* Exp: LP Exp RP  */
#line 509 "./parser.y"
{
    // 9. 括号表达式（强制改变优先级，如(a + b) * c）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1968 "./syntax.tab.c"
    break;

  case 61: /* Exp: MINUS Exp  */
#line 515 "./parser.y"
{
    // 10. 负号表达式（如-3、-a，%prec UMINUS指定为单目运算符）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1978 "./syntax.tab.c"
    break;

  case 62: /* Exp: NOT Exp  */
#line 521 "./parser.y"
{
    // 11. 逻辑非表达式（如!a、!(b > 0)）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 2, (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1988 "./syntax.tab.c"
    break;

  case 63: /* Exp: ID LP Args RP  */
#line 527 "./parser.y"
{
    // 12. 带参数的函数调用（如add(a, b)、foo(1 + 2)）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 1998 "./syntax.tab.c"
    break;

  case 64: /* Exp: ID LP RP  */
#line 533 "./parser.y"
{
    // 13. 无参数的函数调用（如foo()、init()）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2008 "./syntax.tab.c"
    break;

  case 65: /* Exp: Exp LB Exp RB  */
#line 539 "./parser.y"
{
    // 14. 数组下标访问（如a[5]、b[i + 1]）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 4, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2018 "./syntax.tab.c"
    break;

  case 66: /* Exp: Exp DOT ID  */
#line 545 "./parser.y"
{
    // 15. 结构体成员访问（如s.x、student.age）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2028 "./syntax.tab.c"
    break;

  case 67: /* Exp: ID  */
#line 551 "./parser.y"
{
    // 16. 标识符表达式（变量名，如a、b）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2038 "./syntax.tab.c"
    break;

  case 68: /* Exp: INT  */
#line 557 "./parser.y"
{
    // 17. 整型常量表达式（如123、0、-456）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2048 "./syntax.tab.c"
    break;

  case 69: /* Exp: FLOAT  */
#line 563 "./parser.y"
{
    // 18. 浮点型常量表达式（如3.14、0.5、-2.718）
    (yyval.node) = create_ast_node("Exp", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2058 "./syntax.tab.c"
    break;

  case 70: /* Exp: Exp ASSIGNOP error  */
#line 570 "./parser.y"
{
    // 错误1：赋值表达式右侧不完整（如a = ）
    syntaxError++;
}
#line 2067 "./syntax.tab.c"
    break;

  case 71: /* Exp: Exp AND error  */
#line 575 "./parser.y"
{
    // 错误2：逻辑与右侧不完整（如a && ）
    syntaxError++;
}
#line 2076 "./syntax.tab.c"
    break;

  case 72: /* Exp: Exp OR error  */
#line 580 "./parser.y"
{
    // 错误3：逻辑或右侧不完整（如a || ）
    syntaxError++;
}
#line 2085 "./syntax.tab.c"
    break;

  case 73: /* Exp: Exp RELOP error  */
#line 585 "./parser.y"
{
    // 错误4：关系表达式右侧不完整（如a > ）
    syntaxError++;
}
#line 2094 "./syntax.tab.c"
    break;

  case 74: /* Exp: Exp PLUS error  */
#line 590 "./parser.y"
{
    // 错误5：加法表达式右侧不完整（如a + ）
    syntaxError++;
}
#line 2103 "./syntax.tab.c"
    break;

  case 75: /* Exp: Exp MINUS error  */
#line 595 "./parser.y"
{
    // 错误6：减法表达式右侧不完整（如a - ）
    syntaxError++;
}
#line 2112 "./syntax.tab.c"
    break;

  case 76: /* Exp: Exp STAR error  */
#line 600 "./parser.y"
{
    // 错误7：乘法表达式右侧不完整（如a * ）
    syntaxError++;
}
#line 2121 "./syntax.tab.c"
    break;

  case 77: /* Exp: Exp DIV error  */
#line 605 "./parser.y"
{
    // 错误8：除法表达式右侧不完整（如a / ）
    syntaxError++;
}
#line 2130 "./syntax.tab.c"
    break;

  case 78: /* Exp: LP error RP  */
#line 610 "./parser.y"
{
    // 错误9：括号内表达式错误（如(a b)）
    syntaxError++;
}
#line 2139 "./syntax.tab.c"
    break;

  case 79: /* Exp: MINUS error  */
#line 615 "./parser.y"
{
    // 错误10：负号后无表达式（如- ）
    syntaxError++;
}
#line 2148 "./syntax.tab.c"
    break;

  case 80: /* Exp: NOT error  */
#line 620 "./parser.y"
{
    // 错误11：逻辑非后无表达式（如! ）
    syntaxError++;
}
#line 2157 "./syntax.tab.c"
    break;

  case 81: /* Exp: ID LP error RP  */
#line 625 "./parser.y"
{
    // 错误12：函数参数列表错误（如foo(a b)）
    syntaxError++;
}
#line 2166 "./syntax.tab.c"
    break;

  case 82: /* Exp: Exp LB error RB  */
#line 630 "./parser.y"
{
    // 错误13：数组下标错误（如a[ ]、a[b c]）
    syntaxError++;
}
#line 2175 "./syntax.tab.c"
    break;

  case 83: /* Args: Exp COMMA Args  */
#line 637 "./parser.y"
{
    // 多个实参（如add(a + b, 3, c)）
    (yyval.node) = create_ast_node("Args", (yyloc).first_line);
    add_child_nodes((yyval.node), 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
}
#line 2185 "./syntax.tab.c"
    break;

  case 84: /* Args: Exp  */
#line 643 "./parser.y"
{
    // 单个实参（如foo(a)、bar(1 + 2)）
    (yyval.node) = create_ast_node("Args", (yyloc).first_line);
    add_child_nodes((yyval.node), 1, (yyvsp[0].node));
}
#line 2195 "./syntax.tab.c"
    break;


#line 2199 "./syntax.tab.c"

      default: break;
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
      yyerror (YY_("syntax error"));
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

  return yyresult;
}

#line 649 "./parser.y"


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

