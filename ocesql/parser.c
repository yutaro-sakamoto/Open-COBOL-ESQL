/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 21 "parser.y"



#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ocesql.h"
#include "ocesqlutil.h"
#include "define.h"

	static void put_exec_list();
	int cb_get_level(int level);
	struct cb_field * cb_build_field_tree(int level, char *name , struct cb_field *last_field);
	int build_picture (const char *str,struct cb_field * pic);
	int check_has_occurs_children(struct cb_field *field);
	int check_host_has_multi_occurs(struct cb_field *field);

	static struct cb_field		*current_field;
	static struct cb_field		*description_field;
	int hostreferenceCount = 0;

	int yyerror(const char *msg)
	{
	  	printmsg("%06d:%s\n", yylineno,msg);
		return 0;
	}



#line 101 "parser.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SELECT = 258,
    SELECTFROM = 259,
    TOKEN = 260,
    CURNAME = 261,
    HOSTTOKEN = 262,
    WORD = 263,
    PICTURE = 264,
    INSERT = 265,
    UPDATE = 266,
    DISCONNECT = 267,
    DELETE = 268,
    EXECUTE = 269,
    OTHERFUNC = 270,
    INTO = 271,
    NUMERIC = 272,
    END_EXEC = 273,
    EXECSQL = 274,
    EXECSQL_INCLUDE = 275,
    PREPARE = 276,
    FROM = 277,
    DECLARE = 278,
    CURSOR = 279,
    FOR = 280,
    WORKINGBEGIN = 281,
    WORKINGEND = 282,
    HOSTVARIANTBEGIN = 283,
    HOSTVARIANTEND = 284,
    INCLUDE_FILE = 285,
    INCLUDE_SQLCA = 286,
    SQLCA = 287,
    IDENTIFIED_BY = 288,
    COMMIT_WORK = 289,
    ROLLBACK_WORK = 290,
    CONNECT = 291,
    USING = 292,
    OPEN = 293,
    CLOSE = 294,
    FETCH = 295,
    TRAILING = 296,
    COMP_1 = 297,
    COMP_2 = 298,
    COMP_3 = 299,
    COMP_5 = 300,
    USAGE = 301,
    SIGN = 302,
    LEADING = 303,
    SEPARATE = 304,
    AT = 305,
    IS = 306,
    ARE = 307,
    VALUE = 308,
    VARYING = 309,
    ALL = 310,
    OCCURS = 311,
    EXTERNAL = 312,
    TIMES = 313,
    CONST = 314,
    WHERECURRENTOF = 315,
    SQL_BYTEA = 316
  };
#endif
/* Tokens.  */
#define SELECT 258
#define SELECTFROM 259
#define TOKEN 260
#define CURNAME 261
#define HOSTTOKEN 262
#define WORD 263
#define PICTURE 264
#define INSERT 265
#define UPDATE 266
#define DISCONNECT 267
#define DELETE 268
#define EXECUTE 269
#define OTHERFUNC 270
#define INTO 271
#define NUMERIC 272
#define END_EXEC 273
#define EXECSQL 274
#define EXECSQL_INCLUDE 275
#define PREPARE 276
#define FROM 277
#define DECLARE 278
#define CURSOR 279
#define FOR 280
#define WORKINGBEGIN 281
#define WORKINGEND 282
#define HOSTVARIANTBEGIN 283
#define HOSTVARIANTEND 284
#define INCLUDE_FILE 285
#define INCLUDE_SQLCA 286
#define SQLCA 287
#define IDENTIFIED_BY 288
#define COMMIT_WORK 289
#define ROLLBACK_WORK 290
#define CONNECT 291
#define USING 292
#define OPEN 293
#define CLOSE 294
#define FETCH 295
#define TRAILING 296
#define COMP_1 297
#define COMP_2 298
#define COMP_3 299
#define COMP_5 300
#define USAGE 301
#define SIGN 302
#define LEADING 303
#define SEPARATE 304
#define AT 305
#define IS 306
#define ARE 307
#define VALUE 308
#define VARYING 309
#define ALL 310
#define OCCURS 311
#define EXTERNAL 312
#define TIMES 313
#define CONST 314
#define WHERECURRENTOF 315
#define SQL_BYTEA 316

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 52 "parser.y"

	char *s;
	long int ld;
	struct cb_sql_list	*l;
	struct cb_hostreference_list *h;

#line 282 "parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  129
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

#define YYUNDEFTOK  2
#define YYMAXUTOK   316


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    62,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   125,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   147,   156,   160,   168,   171,   180,   183,   191,   192,
     197,   202,   209,   213,   216,   217,   218,   221,   222,   223,
     226,   232,   236,   241,   242,   245,   250,   251,   252,   253,
     256,   262,   267,   272,   277,   282,   287,   292,   295,   300,
     306,   313,   314,   317,   320,   323,   326,   329,   330,   331,
     334,   341,   343,   344,   345,   346,   348,   348,   361,   362,
     363,   364,   365,   366,   367,   371,   371,   386,   386,   404,
     405,   410,   411,   412,   413,   414,   415,   416,   417,   421,
     425,   426,   430,   431,   432,   433,   437,   441,   456,   469,
     471,   472,   473,   476,   480,   486,   487,   489,   490,   494,
     501,   504,   504,   505,   505,   505,   506,   506,   507,   507
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SELECT", "SELECTFROM", "TOKEN",
  "CURNAME", "HOSTTOKEN", "WORD", "PICTURE", "INSERT", "UPDATE",
  "DISCONNECT", "DELETE", "EXECUTE", "OTHERFUNC", "INTO", "NUMERIC",
  "END_EXEC", "EXECSQL", "EXECSQL_INCLUDE", "PREPARE", "FROM", "DECLARE",
  "CURSOR", "FOR", "WORKINGBEGIN", "WORKINGEND", "HOSTVARIANTBEGIN",
  "HOSTVARIANTEND", "INCLUDE_FILE", "INCLUDE_SQLCA", "SQLCA",
  "IDENTIFIED_BY", "COMMIT_WORK", "ROLLBACK_WORK", "CONNECT", "USING",
  "OPEN", "CLOSE", "FETCH", "TRAILING", "COMP_1", "COMP_2", "COMP_3",
  "COMP_5", "USAGE", "SIGN", "LEADING", "SEPARATE", "AT", "IS", "ARE",
  "VALUE", "VARYING", "ALL", "OCCURS", "EXTERNAL", "TIMES", "CONST",
  "WHERECURRENTOF", "SQL_BYTEA", "'.'", "$accept", "sqlstate_list",
  "sqlstate", "updatesql", "update", "disconnectsql", "disconnect",
  "deletesql", "delete", "insertsql", "insert", "rollbacksql", "commitsql",
  "fetchsql", "fetch", "host_references", "res_host_references",
  "closesql", "opensql", "otherdb", "dbid", "connectsql", "othersql",
  "connect", "identified", "using", "incfile", "includesql", "preparesql",
  "execsql", "selectintosql", "declaresql", "prepared_stname",
  "statement_id", "select", "declare_for", "token_list", "host_reference",
  "expr", "sqlvariantstates", "$@1", "sqlvariantstate_list",
  "sqlvariantstate", "$@2", "$@3", "data_description_clause_sequence",
  "data_description_clause", "picture_clause", "usage_clause", "usage",
  "varying_clause", "sqlbytea_clause", "value_clause", "const_clause",
  "sign_clause", "_sign_is", "flag_separate", "occurs_clause",
  "external_clause", "_is", "_is_are", "_all", "_times", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    46
};
# endif

#define YYPACT_NINF (-45)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-122)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -45,    72,   -45,   -29,   -21,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,    -1,    24,   185,    32,     1,   -45,
     -45,    24,    17,   -45,   -45,     9,   140,   -45,   -45,   -45,
     -45,    62,   140,    62,   140,    57,    66,   140,   140,   140,
     140,   140,   140,   123,    79,    12,    90,    87,    60,   -45,
     141,   -45,   -45,    90,   103,   -45,   -45,   -45,   -45,    11,
     -45,   -45,     6,    39,   100,    46,   -45,   -45,    42,   114,
     -45,    48,    51,   112,   -45,   113,    90,   140,   118,   136,
     -45,   -45,   119,   151,    63,   -45,   -45,   -45,   -45,   -45,
     -45,    98,   -45,    90,   156,   -45,   -45,   -45,    90,   -45,
     160,   146,   -45,    90,   -45,   -45,   -45,   -45,   -45,    95,
     -45,    23,   -45,   -45,    90,   163,   -45,   -45,   165,   -45,
     145,   -45,   139,   -45,   -45,   167,   -45,   148,   -45,   -45,
     -45,   -45,   -45,   -45,   133,   -45,   140,   -45,   -45,   -45,
     -45,   -45,   -45,   133,   -45,   -45,   -45,   -45,   -45,   -45,
     142,   150,   -45,    52,   -45,   157,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,    20,   -45,   -45,   134,   122,
      44,   -45,   -45,   -45,   137,   144,   154,   154,   -45,   -45,
     -45,   -45,    15,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    43,     0,    76,     3,    18,    19,    17,
      16,    14,    13,    12,    11,    10,     7,    20,     6,     8,
       9,    15,     4,     5,    43,     0,     0,    43,     0,    78,
      71,     0,     0,    51,    45,    44,     0,    28,    22,    24,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    54,
       0,    44,    46,     0,     0,    73,    72,    75,    74,     0,
      67,    63,     0,     0,     0,     0,    31,    30,     0,     0,
      33,     0,     0,     0,    29,     0,     0,     0,     0,     0,
      52,    49,     0,    87,     0,    77,    83,    84,    79,    80,
      81,     0,    48,     0,     0,    69,    68,    58,     0,    50,
       0,     0,    41,     0,    40,    21,    23,    25,    27,     0,
      37,    65,    62,    61,     0,     0,    85,    89,     0,    82,
       0,    70,     0,    34,    64,     0,    66,     0,    38,    32,
      39,    53,    47,    89,    88,    55,     0,    60,    35,    57,
      36,    56,    42,    86,   106,    99,   102,   103,   104,   105,
     121,   115,   122,   123,   107,     0,   108,    90,    91,    92,
     100,    97,    98,    95,    93,     0,    94,    96,     0,     0,
       0,   116,   124,   125,   126,   128,   117,   117,   120,    59,
     101,   127,     0,   129,   119,   118,   114,   113,   111,   110,
     112,   109
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,    91,   102,   -45,   -45,    54,
     176,   -45,   -45,   174,   -45,   -45,   152,   -45,   -45,   -45,
     -45,   -45,   -35,   -45,   -45,   153,   -40,   -23,   -44,   -45,
     -45,   -45,   -45,   -45,   -45,    67,   -45,   -45,   -45,    31,
     -45,   -45,   -45,   -45,   -45,   -45,    27,   -45,   -45,    55,
     -45,   -45,   -45
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     6,     7,    50,     8,    51,     9,    52,    10,
      53,    11,    12,    13,    54,   132,   119,    14,    15,    26,
      35,    16,    17,    27,    58,   125,    18,    99,    19,    20,
      21,    22,    72,   135,    89,    55,    69,   105,    70,    23,
      29,    60,   101,   143,   127,   144,   167,   168,   169,   170,
     171,   172,   173,   201,   174,   175,   196,   176,   177,   178,
     184,   192,   194
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,    33,    73,    78,    79,    80,    30,    24,    74,    28,
      81,    82,    83,    85,    65,    87,    66,    71,    30,    59,
      88,    25,    67,   198,   107,   106,    65,   103,    66,   106,
      30,    34,   199,    90,    67,    62,    68,   106,   106,   106,
      33,   106,    65,   108,    66,    63,    30,   121,    68,    31,
      67,    65,   154,    66,    65,    30,    66,   109,    30,    67,
     112,   186,    67,   120,    68,    56,   115,    71,   187,   116,
     111,   104,     2,    68,   200,    76,    68,   106,    32,   113,
     120,    57,    31,   104,    77,   133,   156,   157,   158,   159,
     133,     3,     4,    28,   128,    86,   140,    30,     5,   104,
     138,   141,    30,   182,   183,    91,   179,   140,   104,   150,
      31,   104,    92,   139,   150,    65,    65,    66,    66,    30,
      30,   102,   110,    67,    67,    65,    65,    66,    66,    30,
     117,   118,   114,    67,    67,   106,   122,    68,    68,    84,
     189,   154,   155,    65,   148,    66,    30,    68,    68,   146,
     138,    67,    30,   148,   123,    30,   124,   149,    93,   126,
     129,    94,   131,   147,    44,    68,   152,   134,    95,    96,
      97,   136,   104,   104,   185,   156,   157,   158,   159,   160,
     161,   142,   104,   145,   162,   151,   163,   164,    36,   165,
    -121,   188,   191,   162,   166,    37,    38,    39,    40,    41,
      42,   181,   193,   195,   137,   130,    43,    61,    44,    64,
     153,   190,    98,   100,   197,   180,     0,     0,     0,    45,
      46,     0,     0,    47,    48,    49
};

static const yytype_int16 yycheck[] =
{
      44,    24,    42,    47,    48,    49,     7,    36,    43,    30,
      50,    51,    52,    53,     3,     3,     5,     5,     7,    18,
      55,    50,    11,     8,    18,    69,     3,    16,     5,    73,
       7,     7,    17,    56,    11,    18,    25,    81,    82,    83,
      63,    85,     3,    37,     5,    36,     7,    87,    25,    50,
      11,     3,     8,     5,     3,     7,     5,    18,     7,    11,
      18,    41,    11,    86,    25,    33,    18,     5,    48,    18,
      24,    60,     0,    25,    59,    18,    25,   121,    24,    37,
     103,    27,    50,    60,    18,   108,    42,    43,    44,    45,
     113,    19,    20,    30,    31,    16,   119,     7,    26,    60,
       5,   124,     7,    51,    52,    18,   146,   130,    60,   132,
      50,    60,    58,    18,   137,     3,     3,     5,     5,     7,
       7,    18,    22,    11,    11,     3,     3,     5,     5,     7,
      18,    18,    18,    11,    11,   179,    18,    25,    25,    16,
      18,     8,     9,     3,     5,     5,     7,    25,    25,     4,
       5,    11,     7,     5,    18,     7,    37,    18,    17,     8,
      62,    20,     6,    18,    23,    25,    18,     7,    27,    28,
      29,    25,    60,    60,    17,    42,    43,    44,    45,    46,
      47,    18,    60,    18,    51,    18,    53,    54,     3,    56,
      57,    57,    55,    51,    61,    10,    11,    12,    13,    14,
      15,    51,    58,    49,   113,   103,    21,    31,    23,    35,
     143,   180,    60,    60,   187,   160,    -1,    -1,    -1,    34,
      35,    -1,    -1,    38,    39,    40
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    64,     0,    19,    20,    26,    65,    66,    68,    70,
      72,    74,    75,    76,    80,    81,    84,    85,    89,    91,
      92,    93,    94,   102,    36,    50,    82,    86,    30,   103,
       7,    50,    82,   100,     7,    83,     3,    10,    11,    12,
      13,    14,    15,    21,    23,    34,    35,    38,    39,    40,
      67,    69,    71,    73,    77,    98,    33,    82,    87,    18,
     104,    83,    18,    36,    86,     3,     5,    11,    25,    99,
     101,     5,    95,    99,    95,   101,    18,    18,   101,   101,
     101,    99,    99,    99,    16,    99,    16,     3,    95,    97,
     100,    18,    82,    17,    20,    27,    28,    29,    89,    90,
      98,   105,    18,    16,    60,   100,   101,    18,    37,    18,
      22,    24,    18,    37,    18,    18,    18,    18,    18,    79,
     100,    99,    18,    18,    37,    88,     8,   107,    31,    62,
      79,     6,    78,   100,     7,    96,    25,    78,     5,    18,
     100,   100,    18,   106,   108,    18,     4,    18,     5,    18,
     100,    18,    18,   108,     8,     9,    42,    43,    44,    45,
      46,    47,    51,    53,    54,    56,    61,   109,   110,   111,
     112,   113,   114,   115,   117,   118,   120,   121,   122,    99,
     122,    51,    51,    52,   123,    17,    41,    48,    57,    18,
     112,    55,   124,    58,   125,    49,   119,   119,     8,    17,
      59,   116
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    64,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    73,
      74,    75,    76,    77,    78,    78,    78,    79,    79,    79,
      80,    81,    81,    82,    82,    83,    84,    84,    84,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    92,    93,
      93,    94,    94,    95,    96,    97,    98,    99,    99,    99,
      99,   100,   101,   101,   101,   101,   103,   102,   104,   104,
     104,   104,   104,   104,   104,   106,   105,   107,   105,   108,
     108,   109,   109,   109,   109,   109,   109,   109,   109,   110,
     111,   111,   112,   112,   112,   112,   112,   113,   114,   115,
     116,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     121,   122,   122,   123,   123,   123,   124,   124,   125,   125
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     1,     5,     1,     5,     1,     5,     1,     2,
       4,     4,     6,     2,     1,     2,     2,     1,     2,     2,
       5,     5,     7,     0,     2,     1,     4,     6,     5,     4,
       5,     2,     2,     2,     3,     3,     7,     7,     5,     9,
       7,     5,     5,     1,     1,     2,     4,     1,     2,     2,
       3,     1,     1,     1,     1,     1,     0,     4,     0,     2,
       2,     2,     3,     2,     2,     0,     4,     0,     3,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     3,     3,     1,     2,     0,     1,     3,
       2,     0,     1,     0,     1,     1,     0,     1,     0,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
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
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
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
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 21:
#line 149 "parser.y"
{
	(yyval.l) = cb_add_text_list ((yyvsp[-2].l), (yyvsp[-1].l));
	put_exec_list();
}
#line 1633 "parser.c"
    break;

  case 22:
#line 156 "parser.y"
       {(yyval.l) = cb_text_list_add (NULL, (yyvsp[0].s));}
#line 1639 "parser.c"
    break;

  case 23:
#line 162 "parser.y"
{
	(yyval.l) = cb_add_text_list ((yyvsp[-2].l), (yyvsp[-1].l));
	put_exec_list();
}
#line 1648 "parser.c"
    break;

  case 24:
#line 168 "parser.y"
           {(yyval.l) = cb_text_list_add (NULL, (yyvsp[0].s));}
#line 1654 "parser.c"
    break;

  case 25:
#line 173 "parser.y"
{
	(yyval.l) = cb_add_text_list ((yyvsp[-2].l), (yyvsp[-1].l));
	put_exec_list();
}
#line 1663 "parser.c"
    break;

  case 26:
#line 180 "parser.y"
       {(yyval.l) = cb_text_list_add (NULL, (yyvsp[0].s));}
#line 1669 "parser.c"
    break;

  case 27:
#line 185 "parser.y"
{
	(yyval.l) = cb_add_text_list ((yyvsp[-2].l), (yyvsp[-1].l));
	put_exec_list();
}
#line 1678 "parser.c"
    break;

  case 28:
#line 191 "parser.y"
       {(yyval.l) = cb_text_list_add (NULL, (yyvsp[0].s));}
#line 1684 "parser.c"
    break;

  case 29:
#line 192 "parser.y"
              {(yyval.l) = cb_text_list_add ((yyvsp[-1].l), (yyvsp[0].s));}
#line 1690 "parser.c"
    break;

  case 30:
#line 197 "parser.y"
                                       {
	put_exec_list();
}
#line 1698 "parser.c"
    break;

  case 31:
#line 202 "parser.y"
                                     {
	put_exec_list();
}
#line 1706 "parser.c"
    break;

  case 32:
#line 209 "parser.y"
                                                        {
	put_exec_list();
}
#line 1714 "parser.c"
    break;

  case 33:
#line 213 "parser.y"
           { cb_set_cursorname((yyvsp[0].s));}
#line 1720 "parser.c"
    break;

  case 34:
#line 216 "parser.y"
               {cb_host_list_add (host_reference_list, (yyvsp[0].s));}
#line 1726 "parser.c"
    break;

  case 36:
#line 218 "parser.y"
                                 {cb_host_list_add (host_reference_list, (yyvsp[0].s));}
#line 1732 "parser.c"
    break;

  case 37:
#line 221 "parser.y"
               {cb_res_host_list_add (res_host_reference_list, (yyvsp[0].s));}
#line 1738 "parser.c"
    break;

  case 39:
#line 223 "parser.y"
                                     {cb_res_host_list_add (res_host_reference_list, (yyvsp[0].s));}
#line 1744 "parser.c"
    break;

  case 40:
#line 226 "parser.y"
                                    {
	cb_set_cursorname((yyvsp[-1].s));
	put_exec_list();
}
#line 1753 "parser.c"
    break;

  case 41:
#line 232 "parser.y"
                                   {
	cb_set_cursorname((yyvsp[-1].s));
	put_exec_list();
}
#line 1762 "parser.c"
    break;

  case 42:
#line 236 "parser.y"
                                                           {
	cb_set_cursorname((yyvsp[-3].s));
	put_exec_list();
}
#line 1771 "parser.c"
    break;

  case 44:
#line 242 "parser.y"
          { }
#line 1777 "parser.c"
    break;

  case 45:
#line 245 "parser.y"
          {
	cb_set_dbname((yyvsp[0].s));
}
#line 1785 "parser.c"
    break;

  case 46:
#line 250 "parser.y"
                                 { put_exec_list(); }
#line 1791 "parser.c"
    break;

  case 47:
#line 251 "parser.y"
                                                    { put_exec_list(); }
#line 1797 "parser.c"
    break;

  case 48:
#line 252 "parser.y"
                                   { put_exec_list(); }
#line 1803 "parser.c"
    break;

  case 49:
#line 253 "parser.y"
                                   { put_exec_list(); }
#line 1809 "parser.c"
    break;

  case 50:
#line 256 "parser.y"
                                              {
	(yyval.l) = cb_add_text_list(cb_text_list_add(NULL, (yyvsp[-2].s)), (yyvsp[-1].l));
	put_exec_list();
}
#line 1818 "parser.c"
    break;

  case 51:
#line 262 "parser.y"
                       {
	cb_host_list_add (host_reference_list, (yyvsp[0].s));
}
#line 1826 "parser.c"
    break;

  case 52:
#line 267 "parser.y"
                             {
	cb_host_list_add (host_reference_list, (yyvsp[0].s));
}
#line 1834 "parser.c"
    break;

  case 53:
#line 272 "parser.y"
                     {
	cb_host_list_add (host_reference_list, (yyvsp[0].s));
}
#line 1842 "parser.c"
    break;

  case 54:
#line 277 "parser.y"
                                     {
	put_exec_list();
}
#line 1850 "parser.c"
    break;

  case 55:
#line 282 "parser.y"
                                      {
	put_exec_list();
}
#line 1858 "parser.c"
    break;

  case 56:
#line 287 "parser.y"
                                                                   {
	put_exec_list();
}
#line 1866 "parser.c"
    break;

  case 57:
#line 292 "parser.y"
                                                                       {
	put_exec_list();
}
#line 1874 "parser.c"
    break;

  case 58:
#line 295 "parser.y"
                                                   {
	put_exec_list();
}
#line 1882 "parser.c"
    break;

  case 59:
#line 300 "parser.y"
                                                                                           {
	(yyval.l) = cb_add_text_list(cb_text_list_add(NULL, (yyvsp[-6].s)), (yyvsp[-5].l));
	cb_add_text_list((yyval.l), cb_text_list_add(NULL, (yyvsp[-2].s)));
	cb_add_text_list((yyval.l), (yyvsp[-1].l));
	put_exec_list();
}
#line 1893 "parser.c"
    break;

  case 60:
#line 306 "parser.y"
                                                                       {
	(yyval.l) = cb_add_text_list(cb_text_list_add(NULL, (yyvsp[-4].s)), (yyvsp[-3].l));
	put_exec_list();
}
#line 1902 "parser.c"
    break;

  case 61:
#line 313 "parser.y"
                                            { put_exec_list(); }
#line 1908 "parser.c"
    break;

  case 62:
#line 314 "parser.y"
                                                       { put_exec_list(); }
#line 1914 "parser.c"
    break;

  case 63:
#line 317 "parser.y"
     { cb_set_prepname((yyvsp[0].s)); }
#line 1920 "parser.c"
    break;

  case 64:
#line 320 "parser.y"
         { cb_host_list_add (host_reference_list, (yyvsp[0].s)); }
#line 1926 "parser.c"
    break;

  case 65:
#line 323 "parser.y"
                 { (yyval.l) = cb_add_text_list (cb_text_list_add (NULL, (yyvsp[-1].s)), (yyvsp[0].l));}
#line 1932 "parser.c"
    break;

  case 66:
#line 326 "parser.y"
                        { cb_set_cursorname((yyvsp[-2].s));}
#line 1938 "parser.c"
    break;

  case 67:
#line 329 "parser.y"
                                {      (yyval.l) = cb_text_list_add (NULL, (yyvsp[0].s));}
#line 1944 "parser.c"
    break;

  case 68:
#line 330 "parser.y"
                        {      (yyval.l) = cb_text_list_add ((yyvsp[-1].l), (yyvsp[0].s));}
#line 1950 "parser.c"
    break;

  case 69:
#line 331 "parser.y"
                              {
	(yyval.l) = cb_text_list_add ((yyvsp[-1].l), cb_host_list_add (host_reference_list, (yyvsp[0].s)));
}
#line 1958 "parser.c"
    break;

  case 70:
#line 334 "parser.y"
                                    {
	     (yyval.l) = cb_text_list_add((yyvsp[-2].l), "WHERE CURRENT OF");
	     cb_set_cursorname((yyvsp[0].s));
	     (yyval.l) = cb_text_list_add((yyvsp[-2].l), cursorname);
}
#line 1968 "parser.c"
    break;

  case 71:
#line 341 "parser.y"
          {}
#line 1974 "parser.c"
    break;

  case 72:
#line 343 "parser.y"
            {}
#line 1980 "parser.c"
    break;

  case 73:
#line 344 "parser.y"
       {}
#line 1986 "parser.c"
    break;

  case 74:
#line 345 "parser.y"
     {}
#line 1992 "parser.c"
    break;

  case 75:
#line 346 "parser.y"
        {}
#line 1998 "parser.c"
    break;

  case 76:
#line 348 "parser.y"
                               {
	current_field = NULL;
	description_field = NULL;
	put_exec_list();
}
#line 2008 "parser.c"
    break;

  case 77:
#line 355 "parser.y"
           {
	// check host_variable
	put_exec_list();
}
#line 2017 "parser.c"
    break;

  case 83:
#line 366 "parser.y"
                                       { put_exec_list(); }
#line 2023 "parser.c"
    break;

  case 84:
#line 367 "parser.y"
                                     { put_exec_list(); }
#line 2029 "parser.c"
    break;

  case 85:
#line 371 "parser.y"
             {
	struct cb_field *x;

	x =  cb_build_field_tree( (yyvsp[-1].ld), (yyvsp[0].s) , current_field);
	if( x != NULL)
	{
		if( x->level != 78)
			current_field = x;
	}
}
#line 2044 "parser.c"
    break;

  case 86:
#line 382 "parser.y"
{
	if (description_field == NULL)
		description_field = current_field;
}
#line 2053 "parser.c"
    break;

  case 87:
#line 386 "parser.y"
         {
	struct cb_field *x;

	x =  cb_build_field_tree( (yyvsp[0].ld), "" , current_field); // regist dummy name
	if( x != NULL){
		if( x->level != 78)
			current_field = x;
	}
}
#line 2067 "parser.c"
    break;

  case 88:
#line 396 "parser.y"
{
	if (description_field == NULL)
		description_field = current_field;
}
#line 2076 "parser.c"
    break;

  case 89:
#line 404 "parser.y"
{}
#line 2082 "parser.c"
    break;

  case 90:
#line 406 "parser.y"
{}
#line 2088 "parser.c"
    break;

  case 99:
#line 421 "parser.y"
                {  build_picture( (yyvsp[0].s),current_field); }
#line 2094 "parser.c"
    break;

  case 102:
#line 430 "parser.y"
                        { current_field->usage = USAGE_FLOAT;   }
#line 2100 "parser.c"
    break;

  case 103:
#line 431 "parser.y"
                                { current_field->usage = USAGE_DOUBLE; }
#line 2106 "parser.c"
    break;

  case 104:
#line 432 "parser.y"
                                { current_field->usage = USAGE_PACKED; }
#line 2112 "parser.c"
    break;

  case 105:
#line 433 "parser.y"
         {
	printmsg("parse error: Open-COBOL-ESQL does not support 'COMP-5'\n");
	exit(-1);
}
#line 2121 "parser.c"
    break;

  case 106:
#line 437 "parser.y"
                    { current_field->usage = USAGE_OTHER; }
#line 2127 "parser.c"
    break;

  case 107:
#line 442 "parser.y"
{
	if(current_field->pictype != PIC_ALPHANUMERIC &&
		current_field->pictype != PIC_NATIONAL){
		printmsg("parse error: %s specified the data types are not available to VARYING\n",
		       current_field->sname);
		exit(-1);
	}

	var_varying = current_field;
	put_exec_list();
}
#line 2143 "parser.c"
    break;

  case 108:
#line 457 "parser.y"
{
	if(current_field->pictype != PIC_ALPHANUMERIC){
		printf("parse error: current_field->pictype = %d \n",current_field->pictype);
		printf("parse error: %s specified the data types are not available to SQL-BYTEA\n",
		       current_field->sname);
		exit(-1);
	}

	var_sqlbytea = current_field;
	put_exec_list();
}
#line 2159 "parser.c"
    break;

  case 109:
#line 469 "parser.y"
                                              {}
#line 2165 "parser.c"
    break;

  case 110:
#line 471 "parser.y"
                      {}
#line 2171 "parser.c"
    break;

  case 111:
#line 472 "parser.y"
      {}
#line 2177 "parser.c"
    break;

  case 112:
#line 473 "parser.y"
       {}
#line 2183 "parser.c"
    break;

  case 113:
#line 477 "parser.y"
{
	current_field->sign_leading = SIGNLEADING;
}
#line 2191 "parser.c"
    break;

  case 114:
#line 481 "parser.y"
{

}
#line 2199 "parser.c"
    break;

  case 115:
#line 486 "parser.y"
                       {}
#line 2205 "parser.c"
    break;

  case 116:
#line 487 "parser.y"
          {}
#line 2211 "parser.c"
    break;

  case 118:
#line 490 "parser.y"
           { current_field->separate = SIGN_SEPARATE; }
#line 2217 "parser.c"
    break;

  case 119:
#line 495 "parser.y"
{
	current_field->occurs = (int)(yyvsp[-1].ld);
}
#line 2225 "parser.c"
    break;

  case 120:
#line 501 "parser.y"
             {}
#line 2231 "parser.c"
    break;


#line 2235 "parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
                      yytoken, &yylval);
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 509 "parser.y"



static void
put_exec_list()
{
	struct cb_exec_list *l;
	struct cb_exec_list *p;

	struct cb_hostreference_list *h;
	h = host_reference_list;

	for(; h; h = h->next)
	{
		hostreferenceCount++;
	}

	l = malloc (sizeof (struct cb_exec_list));

	l->startLine = startlineno;
	l->endLine = endlineno;
	l->period = period;
	l->host_list = host_reference_list;
	l->hostreferenceCount =hostreferenceCount;
	l->res_host_list = res_host_reference_list;
	l->conn_use_other_db = conn_use_other_db;
	l->sql_list = sql_list;
	l->dbName = com_strdup(dbname);
	l->prepareName = com_strdup(prepname);
	l->cursorName = com_strdup(cursorname);
	l->commandName = com_strdup(commandname);
	l->command_putother = command_putother;
	l->sqlName = com_strdup(sqlname);
	l->incfileName = com_strdup(incfilename);
	l->varname = var_varying;
	l->sqlbyteaname = var_sqlbytea;
	l->next = NULL;

	if (exec_list == NULL)
	{
		exec_list = l;
	}else{
		p = exec_list;
		for (; p->next ; p = p->next);
		p->next = l;

	}

}


static  int  xxx =0;

struct cb_field *getfieldbynamefrom(char *name , struct cb_field *field)
{
	struct cb_field * p;

	if( field == NULL)
		return NULL;

	xxx++;

	if(strcmp(name,field->sname) == 0 ){
		return field;
	}

	p = getfieldbynamefrom(name, field->sister);
	if( p != NULL )
		return p;


	p = getfieldbynamefrom(name, field->children);
	if( p != NULL )
		return p;

	return NULL;

}

struct cb_field * getfieldbyname(char *name )
{
	return getfieldbynamefrom(name, description_field);
}

int gethostvarianttype(char *name,  int *type, int *digits, int *scale)
{
	struct cb_field * p;
	int tmp_type,tmp_dig,tmp_scl;
	p = getfieldbyname(name);
	if( p == NULL){
		return ERR_NOTDEF_WORKING;
	}
	*digits = tmp_dig = p->picnsize;
	*scale = tmp_scl = -(p->scale);
	if(  p->pictype != 0 ){
		switch(p->pictype){
		case PIC_ALPHANUMERIC:
			tmp_type =  HVARTYPE_ALPHABETIC;
			break;
		case PIC_NATIONAL:
			tmp_type = HVARTYPE_NATIONAL;
			break;
		case PIC_NUMERIC:
			if(p->have_sign){
				if(p->usage){
					switch(p->usage){
					case USAGE_PACKED:
						tmp_type = HVARTYPE_SIGNED_PACKED;
						break;
					case USAGE_BINARY_NATIVE:
						tmp_type = HVARTYPE_SIGNED_BINARY_NATIVE;
						break;
					default:
						return ERR_NOT_SUPPORTED_USAGE;
					}
				}else if(p->sign_leading){
					if(p->separate){
						tmp_type = HVARTYPE_SIGNED_LEADING_SEPARATE;
					}else{
						tmp_type = HVARTYPE_SIGNED_LEADING_COMBINED;
					}
				}else{
					if(p->separate){
						tmp_type = HVARTYPE_SIGNED_TRAILING_SEPARATE;
					}else{
						tmp_type = HVARTYPE_SIGNED_TRAILING_COMBINED;
					}
				}
			}else{
				if(p->usage){
					switch(p->usage){
					case USAGE_PACKED:
						tmp_type = HVARTYPE_UNSIGNED_PACKED;
						break;
					case USAGE_BINARY_NATIVE:
						tmp_type = HVARTYPE_UNSIGNED_BINARY_NATIVE;
						break;
					default:
						return ERR_NOT_SUPPORTED_USAGE;
					}
				}else{
					tmp_type = HVARTYPE_UNSIGNED_NUMERIC;
				}
			}
			break;
		case PIC_ALPHANUMERIC_VARYING:
			tmp_type =  HVARTYPE_ALPHANUMERIC_VARYING;
			break;
		case PIC_NATIONAL_VARYING:
			tmp_type =  HVARTYPE_JAPANESE_VARYING;
			break;
		case PIC_SQL_BYTEA:
			tmp_type = HVARTYPE_SQLBYTEA;
			break;
		default:
			break;
		}
		*type = tmp_type;
		return 0;
	} else { // Group data
		if(p->occurs > 0){
			struct cb_field * c;

			c = p->children;
			while(c != NULL){
				if(c->children){
					return ERR_NOTDEF_CONVERSION;
				}
				c = c->sister;
			}
		}
		*type = HVARTYPE_GROUP;
		return 0;
	}
	if(p->usage){
		switch(p->usage){
		case USAGE_FLOAT:
			tmp_type = HVARTYPE_FLOAT;
			break;
		case USAGE_DOUBLE:
			tmp_type = HVARTYPE_FLOAT;
			break;
		default:
			return ERR_NOT_SUPPORTED_USAGE;
		}
		*type = tmp_type;
		return 0;
	}
	return ERR_NOTDEF_CONVERSION;
}

int cb_get_level (int val)
{
	int level = val;

	/* check level */
	switch (level) {
	case 66:
	case 77:
	case 78:
	case 88:
		break;
	default:
		if (level < 1 || level > 49) {
			goto level_error;
		}
		break;
	}

	return level;

	level_error:

	return 0;
}

struct cb_field *
cb_field_founder (struct cb_field *f)
{
     while (f->parent) {
		f = f->parent;
	}
	return f;
}
struct cb_field * cb_build_field_tree(int level, char *name , struct cb_field *last_field)
{
	int lv;
	struct cb_field *f, *p;

	if(name == NULL)
		return NULL;

	lv = cb_get_level (level);
	if (!lv) {
		return NULL;
	}

	f = malloc(sizeof(struct  cb_field));
	if( f == NULL )
		return NULL;

	memset(f, 0 ,sizeof(struct cb_field));

	f->sname = com_strdup(name);

	if (lv == 78) {
		f->level = 1;
	} else{
		f->level = lv;
	}

	if (last_field) {
		if (last_field->level == 77 && f->level != 01 &&
				f->level != 77 && f->level != 66 && f->level != 88) {
			return NULL;
		}
	}

	if (f->level == 1 || f->level == 77) {
		/* top level */
		if (last_field) {
			cb_field_founder (last_field)->sister = f;
		}
	} else {
		if(last_field == NULL){
			printmsg("parse error: %s level should start from 01 or 66 or 77 or 88\n", name);
			exit(-1);
			return NULL;
		}

		if (f->level == 66) {
			/* level 66 */
			f->parent = cb_field_founder (last_field);
			for (p = f->parent->children; p && p->sister; p = p->sister) ;
			if (p) {
				p->sister = f;
			}
		} else if (f->level == 88) {
			/* level 88 */
			f->parent = last_field;
		}else if (f->level > last_field->level) {
			/* lower level */
			last_field->children = f;
			f->parent = last_field;
		} else if (f->level == last_field->level) {
			/* same level */
			same_level:
			last_field->sister = f;
			f->parent = last_field->parent;
		} else {
			/* upper level */
			for (p = last_field->parent; p; p = p->parent) {
				if (p->level == f->level) {
					last_field = p;
					goto same_level;
				}
				if ( p->level < f->level) {
				     break;
				}
			}
			return NULL;
		}
	}

	return f;
}

int  build_picture (const char *str,struct cb_field * pic){
	const char		*p;

	int			i;
	int			n;
	unsigned char		c;

	int	category = 0;
	int s_count = 0;
	int v_count = 0;
	int idx = 0;
	int digits = 0;
	int scale = 0;
	int allocated = 0;

	if (strlen (str) > 50) {
		return 0;
	}

	for(p = str; *p; p++){
		n=1;
		c=*p;
	while(1){
		while(p[1]==c){
			p++; n++;
		}

		if(p[1] == '('){
			i=0;
			p += 2;
			allocated = 0;
			for(;*p == '0';p++){
				;
			}
			for(;*p != ')';p++){
				if(!isdigit(*p)){
					return 0;
				} else {
					allocated++;
					if(allocated > 9){
						return 0;
					}
					i = i * 10 + (*p - '0');
				}
			}
			if(i==0){
				return 0;
			}
			n+=i-1;
			continue;
		}
		break;
		}


		switch(c){
		case 'X':
			if(s_count | v_count){
				return 0;
			}
			category |=  PIC_ALPHANUMERIC;
			digits += n;
			break;
		case '9':
			category |= PIC_NUMERIC;
			digits += n;
			if(v_count){
				scale += n;
			}
			break;
		case 'N':
			if(s_count | v_count){
				return 0;
			}
			category |=  PIC_NATIONAL;
			digits += n;
			break;
		case 'S':
			category |= PIC_NUMERIC;
			if(category & PIC_ALPHABETIC) {
				return 0;
			}
			s_count += n;
			if(s_count > 1 || idx !=0){
				return 0;
			}
			continue;
		case 'V':
			category |= PIC_NUMERIC;
			if(category & PIC_ALPHABETIC) {
				return 0;
			}
			v_count += n;
			if(v_count > 1){
				return 0;
			}
			break;
		default:
			break;
		}
		idx += sizeof(int);
	}

	pic->picnsize = digits;
	pic->scale = scale;
	pic->have_sign = (unsigned char)s_count;
	pic->pictype = category;
	return 1;
}

int
check_has_occurs_children(struct cb_field *field){
	int ret;

	if(field == NULL)
		return 0;

	printmsg("CHILDR:sname=%s, level=%d, occurs=%d, children=%d",
	       field->sname, field->level, field->occurs, field->children);

	if(field->occurs != 0){
		return 1;
	}

	if(field->children != NULL){
		return 2;
	}

	ret = check_has_occurs_children(field->sister);
	if(ret) return ret;

	return 0;
}

int
check_host_has_multi_occurs(struct cb_field *field){
	int ret;

	if(field == NULL)
		return 0;

	if(field->occurs != 0){
		ret = check_has_occurs_children(field->children);
		if(ret) return ret;
	}

	ret = check_host_has_multi_occurs(field->children);
	if(ret) return ret;

	ret = check_host_has_multi_occurs(field->sister);
	if(ret) return ret;

	return 0;
}

