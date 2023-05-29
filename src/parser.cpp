/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEMI = 258,
     COMMA = 259,
     DOT = 260,
     ELLIPSES = 261,
     SQUOTE = 262,
     DQUOTE = 263,
     QUES = 264,
     COLON = 265,
     LPAREN = 266,
     RPAREN = 267,
     LBRACKET = 268,
     RBRACKET = 269,
     LBRACE = 270,
     RBRACE = 271,
     SHLEQ = 272,
     SHL = 273,
     SHREQ = 274,
     SHR = 275,
     EQ_ = 276,
     GE = 277,
     GT_ = 278,
     LEQ = 279,
     LT_ = 280,
     NEQ = 281,
     NOT = 282,
     ASSIGN = 283,
     AND = 284,
     BANDEQ = 285,
     BAND = 286,
     OR = 287,
     BOREQ = 288,
     BOR = 289,
     ARROW = 290,
     BXOREQ = 291,
     BXOR = 292,
     BNOT = 293,
     DADD = 294,
     ADDEQ = 295,
     ADD = 296,
     DSUB = 297,
     SUBEQ = 298,
     SUB = 299,
     MULEQ = 300,
     MUL = 301,
     DIVEQ = 302,
     DIV = 303,
     MODEQ = 304,
     MOD = 305,
     FUNC = 306,
     STRUCT = 307,
     TYPEDEF = 308,
     TYPECAST = 309,
     THIS = 310,
     CONST = 311,
     PTR = 312,
     ARRAY = 313,
     DPTR = 314,
     ADDR = 315,
     IF = 316,
     ELSE = 317,
     FOR = 318,
     SWITCH = 319,
     CASE = 320,
     DEFAULT = 321,
     BREAK = 322,
     CONTINUE = 323,
     RETURN = 324,
     SIZEOF = 325,
     TRUE = 326,
     FALSE = 327,
     NULL_ = 328,
     BOOL = 329,
     SHORT = 330,
     INT = 331,
     LONG = 332,
     CHAR = 333,
     FLOAT = 334,
     DOUBLE = 335,
     VOID = 336,
     AUTO = 337,
     INTEGER = 338,
     IDENTIFIER = 339,
     REAL = 340,
     CHARACTER = 341,
     STRING = 342,
     FUNC_CALL_ARG_LIST = 343,
     GEQ = 344
   };
#endif
/* Tokens.  */
#define SEMI 258
#define COMMA 259
#define DOT 260
#define ELLIPSES 261
#define SQUOTE 262
#define DQUOTE 263
#define QUES 264
#define COLON 265
#define LPAREN 266
#define RPAREN 267
#define LBRACKET 268
#define RBRACKET 269
#define LBRACE 270
#define RBRACE 271
#define SHLEQ 272
#define SHL 273
#define SHREQ 274
#define SHR 275
#define EQ_ 276
#define GE 277
#define GT_ 278
#define LEQ 279
#define LT_ 280
#define NEQ 281
#define NOT 282
#define ASSIGN 283
#define AND 284
#define BANDEQ 285
#define BAND 286
#define OR 287
#define BOREQ 288
#define BOR 289
#define ARROW 290
#define BXOREQ 291
#define BXOR 292
#define BNOT 293
#define DADD 294
#define ADDEQ 295
#define ADD 296
#define DSUB 297
#define SUBEQ 298
#define SUB 299
#define MULEQ 300
#define MUL 301
#define DIVEQ 302
#define DIV 303
#define MODEQ 304
#define MOD 305
#define FUNC 306
#define STRUCT 307
#define TYPEDEF 308
#define TYPECAST 309
#define THIS 310
#define CONST 311
#define PTR 312
#define ARRAY 313
#define DPTR 314
#define ADDR 315
#define IF 316
#define ELSE 317
#define FOR 318
#define SWITCH 319
#define CASE 320
#define DEFAULT 321
#define BREAK 322
#define CONTINUE 323
#define RETURN 324
#define SIZEOF 325
#define TRUE 326
#define FALSE 327
#define NULL_ 328
#define BOOL 329
#define SHORT 330
#define INT 331
#define LONG 332
#define CHAR 333
#define FLOAT 334
#define DOUBLE 335
#define VOID 336
#define AUTO 337
#define INTEGER 338
#define IDENTIFIER 339
#define REAL 340
#define CHARACTER 341
#define STRING 342
#define FUNC_CALL_ARG_LIST 343
#define GEQ 344




/* Copy the first part of user declarations.  */
#line 1 "Parser.y"

#include "ast.hpp" 
#include "include/util.h"
#include <string>
#include <iostream>

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

void yyerror(char *s) {
    extern int yylineno;
    PrintError("[Syntax Error]: " + std::string(s) + " at line " + std::to_string(yylineno));
	std::exit(1); 
}

int yylex(void);

ast::Program *Root;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 23 "Parser.y"
{
    int iVal;
    std::string *sVal;
    double dVal;
    char cVal;
	std::string *strVal;
    ast::Program *program;
    ast::Decl *decl;
    ast::Decls *decls; 
    ast::FuncDecl *funcDecl;
    ast::FuncBody *funcBody;
    ast::VarDecl *varDecl;
    ast::TypeDecl *typeDecl;
    ast::VarType *varType;
    ast::BuiltInType* builtInType;
    ast::FieldDecls* fieldDecls;
	ast::FieldDecl* fieldDecl;
	ast::MemList* memList;
    ast::Stmt *stmt;
    ast::IfStmt *ifStmt;
    ast::ForStmt *forStmt;
    ast::SwitchStmt *switchStmt;
    ast::CaseList *caseList;
    ast::CaseStmt *caseStmt;
    ast::BreakStmt* breakStmt;
	ast::ContinueStmt* continueStmt;
    ast::ReturnStmt *returnStmt;
    ast::Stmts *stmts;
    ast::Block *block;
    ast::Arg *arg;
    ast::ArgList *argList;
    ast::VarInit *varInit;
    ast::VarList *varList;
    ast::Expr *expr;
    ast::Constant* constant;
	ast::ExprList* exprList;
}
/* Line 193 of yacc.c.  */
#line 332 "parser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 345 "parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  90
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  151
/* YYNRULES -- Number of states.  */
#define YYNSTATES  278

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   344

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    13,    15,    24,
      33,    44,    55,    59,    63,    67,    69,    70,    74,    76,
      78,    82,    87,    89,    92,    94,    99,   104,   111,   113,
     115,   117,   119,   121,   123,   125,   127,   129,   131,   134,
     135,   139,   141,   145,   147,   148,   152,   154,   158,   162,
     164,   166,   167,   171,   173,   176,   178,   182,   185,   186,
     189,   191,   193,   195,   197,   199,   201,   203,   205,   207,
     209,   215,   219,   227,   234,   238,   241,   247,   250,   251,
     256,   260,   263,   266,   269,   273,   278,   283,   288,   293,
     298,   305,   312,   316,   320,   323,   326,   333,   336,   339,
     342,   345,   350,   355,   358,   361,   365,   369,   373,   377,
     381,   385,   389,   393,   397,   401,   405,   409,   413,   417,
     421,   425,   429,   433,   439,   443,   447,   451,   455,   459,
     463,   467,   471,   475,   479,   483,   487,   489,   491,   495,
     497,   501,   503,   504,   508,   510,   512,   514,   516,   518,
     520,   522
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      91,     0,    -1,    92,    -1,    92,    93,    -1,    -1,    94,
      -1,    96,    -1,   100,    -1,    51,    84,    11,   108,    12,
      35,   101,     3,    -1,    51,    84,    11,   108,    12,    35,
     101,    95,    -1,    51,    84,    10,    84,    11,   108,    12,
      35,   101,     3,    -1,    51,    84,    10,    84,    11,   108,
      12,    35,   101,    95,    -1,    15,   112,    16,    -1,   101,
      97,     3,    -1,    98,     4,    99,    -1,    99,    -1,    -1,
      98,     4,    99,    -1,    99,    -1,    84,    -1,    84,    28,
     122,    -1,    53,    84,   101,     3,    -1,   102,    -1,    56,
     102,    -1,   103,    -1,    52,    15,   104,    16,    -1,    57,
      25,   101,    23,    -1,    58,    25,   101,     4,    83,    23,
      -1,    82,    -1,    84,    -1,    81,    -1,    74,    -1,    75,
      -1,    76,    -1,    77,    -1,    78,    -1,    79,    -1,    80,
      -1,   104,   105,    -1,    -1,   101,   106,     3,    -1,     3,
      -1,   107,     4,    84,    -1,    84,    -1,    -1,   107,     4,
      84,    -1,    84,    -1,   109,     4,   110,    -1,   109,     4,
       6,    -1,   110,    -1,     6,    -1,    -1,   109,     4,   110,
      -1,   110,    -1,   101,    84,    -1,   101,    -1,    15,   112,
      16,    -1,   112,   113,    -1,    -1,   122,     3,    -1,   114,
      -1,   115,    -1,   116,    -1,   120,    -1,   119,    -1,   121,
      -1,   111,    -1,    96,    -1,   100,    -1,     3,    -1,    61,
     122,   111,    62,   111,    -1,    61,   122,   111,    -1,    63,
     122,     3,   122,     3,   122,   111,    -1,    63,    96,   122,
       3,   122,   111,    -1,    63,   122,   111,    -1,    63,   111,
      -1,    64,   122,    15,   117,    16,    -1,   117,   118,    -1,
      -1,    65,   122,    10,   112,    -1,    66,    10,   112,    -1,
      68,     3,    -1,    67,     3,    -1,    69,     3,    -1,    69,
     122,     3,    -1,   122,    13,   122,    14,    -1,    70,    11,
      84,    12,    -1,    70,    11,   122,    12,    -1,    70,    11,
     101,    12,    -1,    84,    11,   123,    12,    -1,   122,     5,
      84,    11,   123,    12,    -1,   122,    35,    84,    11,   123,
      12,    -1,   122,     5,    84,    -1,   122,    35,    84,    -1,
      41,   122,    -1,    44,   122,    -1,    54,    11,   122,     4,
     101,    12,    -1,    39,   122,    -1,   122,    39,    -1,    42,
     122,    -1,   122,    42,    -1,    59,    11,   122,    12,    -1,
      60,    11,   122,    12,    -1,    27,   122,    -1,    38,   122,
      -1,   122,    48,   122,    -1,   122,    46,   122,    -1,   122,
      50,   122,    -1,   122,    41,   122,    -1,   122,    44,   122,
      -1,   122,    18,   122,    -1,   122,    20,   122,    -1,   122,
      23,   122,    -1,   122,    89,   122,    -1,   122,    25,   122,
      -1,   122,    24,   122,    -1,   122,    21,   122,    -1,   122,
      26,   122,    -1,   122,    31,   122,    -1,   122,    37,   122,
      -1,   122,    34,   122,    -1,   122,    29,   122,    -1,   122,
      32,   122,    -1,   122,     9,   122,    10,   122,    -1,   122,
      28,   122,    -1,   122,    47,   122,    -1,   122,    45,   122,
      -1,   122,    49,   122,    -1,   122,    40,   122,    -1,   122,
      43,   122,    -1,   122,    17,   122,    -1,   122,    19,   122,
      -1,   122,    30,   122,    -1,   122,    36,   122,    -1,   122,
      33,   122,    -1,    11,   122,    12,    -1,    84,    -1,   125,
      -1,   122,     4,   122,    -1,    55,    -1,   124,     4,   122,
      -1,   122,    -1,    -1,   124,     4,   122,    -1,   122,    -1,
      71,    -1,    72,    -1,    73,    -1,    86,    -1,    83,    -1,
      85,    -1,    87,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   132,   132,   135,   136,   139,   140,   141,   144,   145,
     146,   147,   150,   153,   156,   157,   158,   161,   162,   165,
     166,   169,   172,   173,   176,   177,   178,   179,   180,   181,
     184,   185,   186,   187,   188,   189,   190,   191,   194,   195,
     198,   199,   202,   203,   204,   207,   208,   211,   212,   213,
     214,   215,   218,   219,   222,   223,   226,   229,   230,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     246,   247,   250,   251,   252,   253,   256,   259,   260,   263,
     264,   268,   271,   274,   275,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     335,   336,   337,   340,   341,   344,   345,   346,   347,   348,
     349,   350
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMI", "COMMA", "DOT", "ELLIPSES",
  "SQUOTE", "DQUOTE", "QUES", "COLON", "LPAREN", "RPAREN", "LBRACKET",
  "RBRACKET", "LBRACE", "RBRACE", "SHLEQ", "SHL", "SHREQ", "SHR", "EQ_",
  "GE", "GT_", "LEQ", "LT_", "NEQ", "NOT", "ASSIGN", "AND", "BANDEQ",
  "BAND", "OR", "BOREQ", "BOR", "ARROW", "BXOREQ", "BXOR", "BNOT", "DADD",
  "ADDEQ", "ADD", "DSUB", "SUBEQ", "SUB", "MULEQ", "MUL", "DIVEQ", "DIV",
  "MODEQ", "MOD", "FUNC", "STRUCT", "TYPEDEF", "TYPECAST", "THIS", "CONST",
  "PTR", "ARRAY", "DPTR", "ADDR", "IF", "ELSE", "FOR", "SWITCH", "CASE",
  "DEFAULT", "BREAK", "CONTINUE", "RETURN", "SIZEOF", "TRUE", "FALSE",
  "NULL_", "BOOL", "SHORT", "INT", "LONG", "CHAR", "FLOAT", "DOUBLE",
  "VOID", "AUTO", "INTEGER", "IDENTIFIER", "REAL", "CHARACTER", "STRING",
  "FUNC_CALL_ARG_LIST", "GEQ", "$accept", "Program", "Decls", "Decl",
  "FuncDecl", "FuncBody", "VarDecl", "VarList", "_VarList", "VarInit",
  "TypeDecl", "VarType", "_VarType", "BuiltInType", "FieldDecls",
  "FieldDecl", "MemList", "_MemList", "ArgList", "_ArgList", "Arg",
  "Block", "Stmts", "Stmt", "IfStmt", "ForStmt", "SwitchStmt", "CaseList",
  "CaseStmt", "ContinueStmt", "BreakStmt", "ReturnStmt", "Expr",
  "ExprList", "_ExprList", "Constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    90,    91,    92,    92,    93,    93,    93,    94,    94,
      94,    94,    95,    96,    97,    97,    97,    98,    98,    99,
      99,   100,   101,   101,   102,   102,   102,   102,   102,   102,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     105,   105,   106,   106,   106,   107,   107,   108,   108,   108,
     108,   108,   109,   109,   110,   110,   111,   112,   112,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     114,   114,   115,   115,   115,   115,   116,   117,   117,   118,
     118,   119,   120,   121,   121,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     123,   123,   123,   124,   124,   125,   125,   125,   125,   125,
     125,   125
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     8,     8,
      10,    10,     3,     3,     3,     1,     0,     3,     1,     1,
       3,     4,     1,     2,     1,     4,     4,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       3,     1,     3,     1,     0,     3,     1,     3,     3,     1,
       1,     0,     3,     1,     2,     1,     3,     2,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     3,     7,     6,     3,     2,     5,     2,     0,     4,
       3,     2,     2,     2,     3,     4,     4,     4,     4,     4,
       6,     6,     3,     3,     2,     2,     6,     2,     2,     2,
       2,     4,     4,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       3,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    35,    36,    37,    30,    28,    29,
       3,     5,     6,     7,    16,    22,    24,     0,    39,     0,
      23,     0,     0,    19,     0,     0,    15,     0,    51,     0,
       0,     0,     0,     0,    13,     0,     0,    50,    55,     0,
       0,    49,    41,    25,    44,    38,    21,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
       0,   145,   146,   147,   149,   136,   150,   148,   151,    20,
     137,    14,    51,    54,     0,     0,    43,     0,     0,     0,
       0,   103,   104,    97,    94,    99,    95,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    47,    40,     0,    27,   135,     0,     0,     0,   136,
       0,     0,   141,     0,     0,   138,    92,     0,     0,   130,
     110,   131,   111,   116,   112,   115,   114,   117,   124,   121,
     132,   118,   122,   134,   120,    93,   133,   119,   128,   108,
     129,   109,   126,   106,   125,   105,   127,   107,   113,     0,
       0,    42,     0,   101,   102,    86,    88,    87,    89,     0,
     142,     0,    85,   142,     0,     8,    58,     9,   136,     0,
     140,     0,   123,     0,     0,     0,    96,    90,    91,    10,
      11,    69,    58,    12,     0,     0,     0,     0,     0,     0,
      67,    68,    66,    57,    60,    61,    62,    64,    63,    65,
       0,     0,     0,     0,    75,     0,     0,    82,    81,    83,
       0,    59,    56,    71,     0,     0,    74,    78,    84,     0,
       0,     0,     0,    70,     0,     0,    76,     0,     0,    77,
      73,     0,     0,    58,    72,    58,    80,    79
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    20,    21,   207,   230,    34,    35,    36,
     231,    24,    25,    26,    39,    55,    87,    88,    49,    50,
      51,   232,   215,   233,   234,   235,   236,   262,   269,   237,
     238,   239,   240,   153,   154,    80
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -219
static const yytype_int16 yypact[] =
{
    -219,     6,  2009,  -219,   -61,    10,   -52,  2051,     9,    13,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,   -45,  -219,  -219,    -1,  -219,  2020,
    -219,  2020,  2020,    16,    44,    45,    48,   -36,  1637,  1623,
      57,    72,    95,  1955,  -219,   -45,    89,  -219,    20,    93,
     102,   104,  -219,  -219,    25,  -219,  -219,  -219,    29,  1955,
    1955,  1955,  1955,  1955,  1955,  1955,    99,  -219,   103,   109,
     114,  -219,  -219,  -219,  -219,   139,  -219,  -219,  -219,  1277,
    -219,   147,  1637,  -219,   117,  1668,   151,   150,   155,   138,
     414,    -2,    -2,    -2,    -2,    -2,    -2,  1955,  1955,  1955,
    1801,  1955,  1955,    79,  1955,  1955,  1955,  1955,  1955,  1955,
    1955,  1955,  1955,  1955,  1955,  1955,  1955,  1955,  1955,  1955,
    1955,  1955,    80,  1955,  1955,  -219,  1955,  1955,  -219,  1955,
    1955,  1955,  1955,  1955,  1955,  1955,  1955,  1955,   153,  2020,
    -219,   162,  -219,    84,  -219,  -219,   873,   465,   516,     3,
     158,   567,   924,   159,   168,  1277,   163,   618,   669,  1277,
    2008,  1277,  2008,  1617,   149,   149,   149,  1617,  1277,  1401,
    1277,  1579,  1363,  1277,  1501,   164,  1277,  1539,  1277,  1110,
    1277,  1110,  1277,    -2,  1277,    -2,  1277,    -2,   149,   141,
      27,   169,  1878,  -219,  -219,  -219,  -219,  -219,  -219,  1955,
    1955,  1955,  -219,  1955,  2020,  -219,  -219,  -219,     1,   165,
     975,   166,  1323,   167,    28,  1024,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,  1955,  1724,  1955,   177,   182,  1426,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
     210,  1109,   720,  1955,  -219,    98,   771,  -219,  -219,  -219,
     261,  -219,  -219,   130,   312,  1955,  -219,  -219,  -219,   179,
    1955,   363,   -15,  -219,   720,  1955,  -219,  1955,   186,  -219,
    -219,   720,   822,  -219,  -219,  -219,  1194,  1194
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -219,  -219,  -219,  -219,  -219,   -16,     0,  -219,  -219,   157,
     201,    -3,   197,  -219,  -219,  -219,  -219,  -219,   123,  -219,
     121,  -218,  -177,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,   -43,  -195,  -219,  -219
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -145
static const yytype_int16 yytable[] =
{
      79,   266,    22,   103,   -29,   211,     3,   244,   213,    37,
      38,   105,   101,   -29,   101,   195,    90,    91,    92,    93,
      94,    95,    96,    27,   253,    28,    40,   256,    41,    42,
     205,   219,    29,   122,    31,    48,    54,   125,    32,    33,
     128,   263,   206,   206,    43,   241,   270,    44,    46,    45,
     267,   268,   -18,   274,   146,   147,   148,   151,   152,   155,
      56,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,    48,
     176,   177,    48,   178,   179,   -29,   180,   181,   182,   183,
     184,   185,   186,   187,   188,    57,   276,   150,   277,    58,
      82,   255,   102,   103,    83,    84,    85,   104,   -53,    86,
      97,   105,    89,   222,    98,   106,   107,   108,   109,   110,
      99,   111,   112,   113,   114,   100,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   190,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   155,
     101,   -17,   139,   142,   103,   -46,   210,   152,   212,   143,
     152,   144,   105,   156,   175,   189,   -52,   107,   191,   109,
     196,   198,   199,   -45,   200,   203,   204,   216,   217,   218,
     247,   242,   245,   246,   122,   248,   250,   137,   125,   209,
     127,   128,   259,   130,   222,   132,   273,   134,   220,   136,
     254,   214,    81,    23,    30,   138,   141,     0,     0,     0,
       0,     0,   261,   251,   102,   103,     0,   264,     0,   104,
       0,     0,   271,   105,   272,   243,     0,   106,   107,   108,
     109,   110,     0,   111,   112,   113,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,     0,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,     0,     0,     0,   258,   102,   103,     0,     0,     0,
     104,     0,     0,     0,   105,     0,     0,     0,   106,   107,
     108,   109,   110,     0,   111,   112,   113,   114,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   137,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,     0,     0,     0,   260,   102,   103,     0,     0,
       0,   104,     0,     0,     0,   105,     0,     0,     0,   106,
     107,   108,   109,   110,     0,   111,   112,   113,   114,     0,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     137,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,     0,     0,     0,   265,   102,   103,     0,
       0,     0,   104,     0,     0,     0,   105,     0,     0,     0,
     106,   107,   108,   109,   110,     0,   111,   112,   113,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   137,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,     0,     0,     0,     0,   102,   103,
       0,     0,     0,   104,     0,     0,   145,   105,     0,     0,
       0,   106,   107,   108,   109,   110,     0,   111,   112,   113,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   137,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,     0,     0,     0,     0,   102,
     103,     0,     0,     0,   104,     0,     0,   193,   105,     0,
       0,     0,   106,   107,   108,   109,   110,     0,   111,   112,
     113,   114,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   137,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,     0,     0,     0,     0,
     102,   103,     0,     0,     0,   104,     0,     0,   194,   105,
       0,     0,     0,   106,   107,   108,   109,   110,     0,   111,
     112,   113,   114,     0,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   137,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,     0,     0,     0,
       0,   102,   103,     0,     0,     0,   104,     0,     0,   197,
     105,     0,     0,     0,   106,   107,   108,   109,   110,     0,
     111,   112,   113,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   137,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,     0,     0,
       0,     0,   102,   103,     0,     0,     0,   104,   201,     0,
       0,   105,     0,     0,     0,   106,   107,   108,   109,   110,
       0,   111,   112,   113,   114,     0,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   137,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,     0,
       0,     0,     0,   102,   103,     0,     0,     0,   104,     0,
       0,     0,   105,   202,     0,     0,   106,   107,   108,   109,
     110,     0,   111,   112,   113,   114,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   137,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
       0,     0,     0,     0,   102,   103,     0,     0,     0,   104,
       0,     0,     0,   105,     0,   222,     0,   106,   107,   108,
     109,   110,     0,   111,   112,   113,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   137,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,     0,     0,     0,     0,   102,   103,     0,     0,     0,
     104,     0,     0,     0,   105,     0,   257,     0,   106,   107,
     108,   109,   110,     0,   111,   112,   113,   114,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   137,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,     0,     0,     0,     0,   102,   103,     0,     0,
       0,   104,   275,     0,     0,   105,     0,     0,     0,   106,
     107,   108,   109,   110,     0,   111,   112,   113,   114,     0,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     137,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,     0,     0,     0,     0,   192,   103,     0,
       0,     0,   104,     0,     0,     0,   105,     0,     0,     0,
     106,   107,   108,   109,   110,     0,   111,   112,   113,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   137,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,     0,     0,     0,     0,  -144,   103,
       0,     0,     0,   104,     0,     0,     0,   105,     0,     0,
       0,   106,   107,   108,   109,   110,     0,   111,   112,   113,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   137,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,     0,     0,     0,     0,  -143,
     103,     0,     0,     0,   104,     0,     0,     0,   105,     0,
       0,     0,   106,   107,   108,   109,   110,     0,   111,   112,
     113,   114,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   137,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,     0,   221,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,   222,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,   137,    63,    64,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     5,     6,    66,    67,
       7,     8,     9,    68,    69,   224,     0,   225,   226,     0,
       0,   227,   228,   229,    70,    71,    72,    73,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    74,   208,    76,
      77,    78,   221,     0,     0,   103,     0,     0,     0,     0,
      59,     0,     0,   105,   222,   252,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,    61,    62,   125,
      63,    64,   128,    65,     0,     0,   132,     0,   134,     0,
     136,     5,     6,    66,    67,     7,     8,     9,    68,    69,
     224,     0,   225,   226,     0,     0,   227,   228,   229,    70,
      71,    72,    73,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    74,   208,    76,    77,    78,   221,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,    63,    64,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     5,     6,    66,    67,
       7,     8,     9,    68,    69,   224,     0,   225,   226,     0,
       0,   227,   228,   229,    70,    71,    72,    73,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    74,   208,    76,
      77,    78,   103,     0,     0,     0,   104,     0,     0,     0,
     105,     0,     0,     0,   106,   107,   108,   109,   110,     0,
     111,   112,   113,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   103,     0,
       0,     0,   104,     0,     0,     0,   105,     0,     0,     0,
       0,   107,     0,   109,   110,     0,   111,   112,   113,   114,
       0,     0,   116,     0,   118,   119,     0,   121,   122,     0,
     124,     0,   125,     0,   127,   128,   137,   130,   103,   132,
       0,   134,     0,   136,     0,     0,   105,     0,     0,     0,
       0,   107,     0,   109,   110,     0,   111,   112,   113,   114,
       0,     0,   116,     0,   118,     0,     0,   121,   122,     0,
     124,     0,   125,     0,   127,   128,   103,   130,     0,   132,
       0,   134,   137,   136,   105,     0,     0,     0,     0,   107,
       0,   109,   110,     0,   111,   112,   113,   114,     0,   249,
       0,     0,   118,     0,     0,   121,   122,    59,   124,     0,
     125,     0,   127,   128,     0,   130,     0,   132,     0,   134,
       0,   136,   137,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,     0,    63,    64,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,    68,    69,     0,     0,     0,
     137,     0,     0,     0,     0,     0,    70,    71,    72,    73,
       0,     0,     0,     0,     0,     0,   103,     0,     0,    74,
      75,    76,    77,    78,   105,     0,     0,     0,     0,   107,
       0,   109,   110,     0,   111,   112,   113,   114,     0,     0,
       0,     0,   118,     0,     0,     0,   122,     0,   124,     0,
     125,     0,   127,   128,   103,   130,     0,   132,     0,   134,
       0,   136,   105,     0,     0,     0,     0,   107,     0,   109,
     110,     0,   111,   112,   113,   114,     0,     0,     0,     0,
     118,     0,     0,     0,   122,     0,     0,     0,   125,     0,
     127,   128,     0,   130,   103,   132,     0,   134,     0,   136,
     137,     0,   105,     0,     0,     0,     0,   107,     0,   109,
     110,     0,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,     0,     0,   125,     0,
     127,   128,   103,   130,     0,   132,    52,   134,   137,   136,
     105,     0,     0,     0,     0,   107,     0,   109,     0,    53,
     111,   112,   113,    47,     0,     0,     0,     0,     0,     0,
       0,     0,   122,     0,     0,     0,   125,     0,   127,   128,
       0,   130,     0,   132,     0,   134,     0,   136,   137,     0,
       0,     0,     0,     0,   140,     5,     0,     0,     0,     7,
       8,     9,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     0,     7,     8,     9,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,   137,    19,     0,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
       5,    19,     0,     0,     7,     8,     9,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,   222,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    60,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,    63,    64,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     5,     0,    66,    67,
       7,     8,     9,    68,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    74,   208,    76,
      77,    78,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,     0,    63,    64,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     5,     0,    66,    67,     7,     8,     9,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    74,   149,    76,    77,    78,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,     0,    63,
      64,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       5,     0,    66,    67,     7,     8,     9,    68,    69,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    74,   208,    76,    77,    78,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,     0,    63,    64,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,     0,     0,   103,    68,    69,     0,     0,     0,     0,
       0,   105,     0,     0,     0,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   122,     0,     0,     0,   125,     0,   127,
     128,     0,   130,     0,   132,     0,   134,     0,   136,     0,
       4,     5,     6,     0,     0,     7,     8,     9,     0,     0,
       0,     0,     5,     0,     0,     0,     7,     8,     9,     0,
       0,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,    18,     0,    19,    10,    11,    12,    13,    14,    15,
      16,    17,    18,     5,    19,     0,     0,     0,     8,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,     0,    19
};

static const yytype_int16 yycheck[] =
{
      43,    16,     2,     5,     3,   200,     0,   225,   203,    10,
      11,    13,    11,    12,    11,    12,    59,    60,    61,    62,
      63,    64,    65,    84,   242,    15,    29,   245,    31,    32,
       3,     3,    84,    35,    25,    38,    39,    39,    25,    84,
      42,   259,    15,    15,    28,   222,   264,     3,    84,     4,
      65,    66,     4,   271,    97,    98,    99,   100,   101,   102,
       3,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    82,
     123,   124,    85,   126,   127,    84,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    23,   273,   100,   275,     4,
      11,     3,     4,     5,    84,    12,     4,     9,     4,    84,
      11,    13,    83,    15,    11,    17,    18,    19,    20,    21,
      11,    23,    24,    25,    26,    11,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,   139,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   192,
      11,     4,    35,     3,     5,     4,   199,   200,   201,     4,
     203,    23,    13,    84,    84,    12,     4,    18,    84,    20,
      12,    12,     4,     4,    11,    11,    35,    12,    12,    12,
       3,   224,   225,   226,    35,     3,   229,    89,    39,   192,
      41,    42,    62,    44,    15,    46,    10,    48,   214,    50,
     243,   204,    45,     2,     7,    82,    85,    -1,    -1,    -1,
      -1,    -1,   255,     3,     4,     5,    -1,   260,    -1,     9,
      -1,    -1,   265,    13,   267,   225,    -1,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    -1,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    89,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      89,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,     3,     4,     5,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    89,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    -1,    -1,    -1,     4,     5,
      -1,    -1,    -1,     9,    -1,    -1,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    89,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,     4,
       5,    -1,    -1,    -1,     9,    -1,    -1,    12,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    89,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
       4,     5,    -1,    -1,    -1,     9,    -1,    -1,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    89,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      -1,     4,     5,    -1,    -1,    -1,     9,    -1,    -1,    12,
      13,    -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    -1,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    89,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      -1,    -1,     4,     5,    -1,    -1,    -1,     9,    10,    -1,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    89,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    -1,     4,     5,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    89,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    -1,    -1,    -1,     4,     5,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    15,    -1,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    89,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    -1,     4,     5,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    15,    -1,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    89,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,     4,     5,    -1,    -1,
      -1,     9,    10,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      89,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    -1,     4,     5,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    89,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    -1,    -1,    -1,     4,     5,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    89,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,     4,
       5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    89,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    89,    41,    42,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,     3,    -1,    -1,     5,    -1,    -1,    -1,    -1,
      11,    -1,    -1,    13,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    38,    39,    39,
      41,    42,    42,    44,    -1,    -1,    46,    -1,    48,    -1,
      50,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    -1,    -1,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    -1,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     5,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    23,    24,    25,    26,
      -1,    -1,    29,    -1,    31,    32,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    41,    42,    89,    44,     5,    46,
      -1,    48,    -1,    50,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    23,    24,    25,    26,
      -1,    -1,    29,    -1,    31,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    -1,    41,    42,     5,    44,    -1,    46,
      -1,    48,    89,    50,    13,    -1,    -1,    -1,    -1,    18,
      -1,    20,    21,    -1,    23,    24,    25,    26,    -1,     3,
      -1,    -1,    31,    -1,    -1,    34,    35,    11,    37,    -1,
      39,    -1,    41,    42,    -1,    44,    -1,    46,    -1,    48,
      -1,    50,    89,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,    83,
      84,    85,    86,    87,    13,    -1,    -1,    -1,    -1,    18,
      -1,    20,    21,    -1,    23,    24,    25,    26,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,
      39,    -1,    41,    42,     5,    44,    -1,    46,    -1,    48,
      -1,    50,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    42,    -1,    44,     5,    46,    -1,    48,    -1,    50,
      89,    -1,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    42,     5,    44,    -1,    46,     3,    48,    89,    50,
      13,    -1,    -1,    -1,    -1,    18,    -1,    20,    -1,    16,
      23,    24,    25,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    42,
      -1,    44,    -1,    46,    -1,    48,    -1,    50,    89,    -1,
      -1,    -1,    -1,    -1,     6,    52,    -1,    -1,    -1,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    89,    84,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      52,    84,    -1,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    15,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    27,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    55,
      56,    57,    58,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    54,    55,    56,    57,    58,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    -1,    -1,     5,    59,    60,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,    35,    -1,    -1,    -1,    39,    -1,    41,
      42,    -1,    44,    -1,    46,    -1,    48,    -1,    50,    -1,
      51,    52,    53,    -1,    -1,    56,    57,    58,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    56,    57,    58,    -1,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    52,    84,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    91,    92,     0,    51,    52,    53,    56,    57,    58,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    84,
      93,    94,    96,   100,   101,   102,   103,    84,    15,    84,
     102,    25,    25,    84,    97,    98,    99,    10,    11,   104,
     101,   101,   101,    28,     3,     4,    84,     6,   101,   108,
     109,   110,     3,    16,   101,   105,     3,    23,     4,    11,
      27,    38,    39,    41,    42,    44,    54,    55,    59,    60,
      70,    71,    72,    73,    83,    84,    85,    86,    87,   122,
     125,    99,    11,    84,    12,     4,    84,   106,   107,    83,
     122,   122,   122,   122,   122,   122,   122,    11,    11,    11,
      11,    11,     4,     5,     9,    13,    17,    18,    19,    20,
      21,    23,    24,    25,    26,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    89,   108,    35,
       6,   110,     3,     4,    23,    12,   122,   122,   122,    84,
     101,   122,   122,   123,   124,   122,    84,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,    84,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,    12,
     101,    84,     4,    12,    12,    12,    12,    12,    12,     4,
      11,    10,    14,    11,    35,     3,    15,    95,    84,   101,
     122,   123,   122,   123,   101,   112,    12,    12,    12,     3,
      95,     3,    15,    16,    61,    63,    64,    67,    68,    69,
      96,   100,   111,   113,   114,   115,   116,   119,   120,   121,
     122,   112,   122,    96,   111,   122,   122,     3,     3,     3,
     122,     3,    16,   111,   122,     3,   111,    15,     3,    62,
       3,   122,   117,   111,   122,     3,    16,    65,    66,   118,
     111,   122,   122,    10,   111,    10,   112,   112
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 132 "Parser.y"
    {  (yyval.program) = new ast::Program((yyvsp[(1) - (1)].decls)); Root = (yyval.program);   ;}
    break;

  case 3:
#line 135 "Parser.y"
    {  (yyval.decls) = (yyvsp[(1) - (2)].decls); (yyval.decls)->push_back((yyvsp[(2) - (2)].decl));   ;}
    break;

  case 4:
#line 136 "Parser.y"
    {  (yyval.decls) = new ast::Decls();   ;}
    break;

  case 5:
#line 139 "Parser.y"
    {  (yyval.decl) = (yyvsp[(1) - (1)].funcDecl);   ;}
    break;

  case 6:
#line 140 "Parser.y"
    {  (yyval.decl) = (yyvsp[(1) - (1)].varDecl);   ;}
    break;

  case 7:
#line 141 "Parser.y"
    {  (yyval.decl) = (yyvsp[(1) - (1)].typeDecl);   ;}
    break;

  case 8:
#line 144 "Parser.y"
    {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(7) - (8)].varType),*(yyvsp[(2) - (8)].sVal),(yyvsp[(4) - (8)].argList));   ;}
    break;

  case 9:
#line 145 "Parser.y"
    {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(7) - (8)].varType),*(yyvsp[(2) - (8)].sVal),(yyvsp[(4) - (8)].argList),"",(yyvsp[(8) - (8)].funcBody));   ;}
    break;

  case 10:
#line 146 "Parser.y"
    {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(9) - (10)].varType),*(yyvsp[(4) - (10)].sVal),(yyvsp[(6) - (10)].argList),*(yyvsp[(2) - (10)].sVal));   ;}
    break;

  case 11:
#line 147 "Parser.y"
    {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(9) - (10)].varType),*(yyvsp[(4) - (10)].sVal),(yyvsp[(6) - (10)].argList),*(yyvsp[(2) - (10)].sVal),(yyvsp[(10) - (10)].funcBody));   ;}
    break;

  case 12:
#line 150 "Parser.y"
    {  (yyval.funcBody) = new ast::FuncBody((yyvsp[(2) - (3)].stmts));   ;}
    break;

  case 13:
#line 153 "Parser.y"
    {  (yyval.varDecl) = new ast::VarDecl((yyvsp[(1) - (3)].varType),(yyvsp[(2) - (3)].varList));   ;}
    break;

  case 14:
#line 156 "Parser.y"
    {  (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit));   ;}
    break;

  case 15:
#line 157 "Parser.y"
    {  (yyval.varList) = new ast::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit));   ;}
    break;

  case 16:
#line 158 "Parser.y"
    {  (yyval.varList) = new ast::VarList();   ;}
    break;

  case 17:
#line 161 "Parser.y"
    {  (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit));   ;}
    break;

  case 18:
#line 162 "Parser.y"
    {  (yyval.varList) = new ast::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit));   ;}
    break;

  case 19:
#line 165 "Parser.y"
    {  (yyval.varInit) = new ast::VarInit(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 20:
#line 166 "Parser.y"
    {  (yyval.varInit) = new ast::VarInit(*(yyvsp[(1) - (3)].sVal),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 21:
#line 169 "Parser.y"
    {  (yyval.typeDecl) = new ast::TypeDecl(*(yyvsp[(2) - (4)].sVal),(yyvsp[(3) - (4)].varType));   ;}
    break;

  case 22:
#line 172 "Parser.y"
    {  (yyval.varType) = (yyvsp[(1) - (1)].varType);   ;}
    break;

  case 23:
#line 173 "Parser.y"
    {  (yyval.varType) = (yyvsp[(2) - (2)].varType); (yyval.varType)->SetConst();   ;}
    break;

  case 24:
#line 176 "Parser.y"
    {  (yyval.varType) = (yyvsp[(1) - (1)].builtInType);   ;}
    break;

  case 25:
#line 177 "Parser.y"
    {  (yyval.varType) = new ast::StructType((yyvsp[(3) - (4)].fieldDecls));   ;}
    break;

  case 26:
#line 178 "Parser.y"
    {  (yyval.varType) = new ast::PointerType((yyvsp[(3) - (4)].varType));   ;}
    break;

  case 27:
#line 179 "Parser.y"
    {  (yyval.varType) = new ast::ArrayType((yyvsp[(3) - (6)].varType),(yyvsp[(5) - (6)].iVal));   ;}
    break;

  case 28:
#line 180 "Parser.y"
    {  (yyval.varType) = new ast::AutoType();   ;}
    break;

  case 29:
#line 181 "Parser.y"
    {  (yyval.varType) = new ast::DefinedType(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 30:
#line 184 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Void);   ;}
    break;

  case 31:
#line 185 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Bool);   ;}
    break;

  case 32:
#line 186 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Short);   ;}
    break;

  case 33:
#line 187 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Int);   ;}
    break;

  case 34:
#line 188 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Long);   ;}
    break;

  case 35:
#line 189 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Char);   ;}
    break;

  case 36:
#line 190 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Float);   ;}
    break;

  case 37:
#line 191 "Parser.y"
    {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Double);   ;}
    break;

  case 38:
#line 194 "Parser.y"
    {  (yyval.fieldDecls) = (yyvsp[(1) - (2)].fieldDecls); if ((yyvsp[(2) - (2)].fieldDecl) != NULL) (yyval.fieldDecls)->push_back((yyvsp[(2) - (2)].fieldDecl));   ;}
    break;

  case 39:
#line 195 "Parser.y"
    {  (yyval.fieldDecls) = new ast::FieldDecls();   ;}
    break;

  case 40:
#line 198 "Parser.y"
    {  (yyval.fieldDecl) = new ast::FieldDecl((yyvsp[(1) - (3)].varType),(yyvsp[(2) - (3)].memList));   ;}
    break;

  case 41:
#line 199 "Parser.y"
    {  (yyval.fieldDecl) = NULL;   ;}
    break;

  case 42:
#line 202 "Parser.y"
    {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 43:
#line 203 "Parser.y"
    {  (yyval.memList) = new ast::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 44:
#line 204 "Parser.y"
    {  (yyval.memList) = new ast::MemList();   ;}
    break;

  case 45:
#line 207 "Parser.y"
    {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 46:
#line 208 "Parser.y"
    {  (yyval.memList) = new ast::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 47:
#line 211 "Parser.y"
    {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg));   ;}
    break;

  case 48:
#line 212 "Parser.y"
    {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->SetVarArg();   ;}
    break;

  case 49:
#line 213 "Parser.y"
    {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg));   ;}
    break;

  case 50:
#line 214 "Parser.y"
    {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->SetVarArg();   ;}
    break;

  case 51:
#line 215 "Parser.y"
    {  (yyval.argList) = new ast::ArgList();   ;}
    break;

  case 52:
#line 218 "Parser.y"
    {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg));   ;}
    break;

  case 53:
#line 219 "Parser.y"
    {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg));   ;}
    break;

  case 54:
#line 222 "Parser.y"
    {  (yyval.arg) = new ast::Arg((yyvsp[(1) - (2)].varType),*(yyvsp[(2) - (2)].sVal));   ;}
    break;

  case 55:
#line 223 "Parser.y"
    {  (yyval.arg) = new ast::Arg((yyvsp[(1) - (1)].varType));   ;}
    break;

  case 56:
#line 226 "Parser.y"
    {  (yyval.block) = new ast::Block((yyvsp[(2) - (3)].stmts));   ;}
    break;

  case 57:
#line 229 "Parser.y"
    {  (yyval.stmts) = (yyvsp[(1) - (2)].stmts); if ((yyvsp[(2) - (2)].stmt) != NULL) (yyval.stmts)->push_back((yyvsp[(2) - (2)].stmt));   ;}
    break;

  case 58:
#line 230 "Parser.y"
    {  (yyval.stmts) = new ast::Stmts();   ;}
    break;

  case 59:
#line 233 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (2)].expr);   ;}
    break;

  case 60:
#line 234 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].ifStmt);   ;}
    break;

  case 61:
#line 235 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].forStmt);   ;}
    break;

  case 62:
#line 236 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].switchStmt);   ;}
    break;

  case 63:
#line 237 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].breakStmt);   ;}
    break;

  case 64:
#line 238 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].continueStmt);   ;}
    break;

  case 65:
#line 239 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].returnStmt);   ;}
    break;

  case 66:
#line 240 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].block);   ;}
    break;

  case 67:
#line 241 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].varDecl);   ;}
    break;

  case 68:
#line 242 "Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].typeDecl);   ;}
    break;

  case 69:
#line 243 "Parser.y"
    {  (yyval.stmt) = NULL;   ;}
    break;

  case 70:
#line 246 "Parser.y"
    {  (yyval.ifStmt) = new ast::IfStmt((yyvsp[(2) - (5)].expr),(yyvsp[(3) - (5)].block),(yyvsp[(5) - (5)].block));   ;}
    break;

  case 71:
#line 247 "Parser.y"
    {  (yyval.ifStmt) = new ast::IfStmt((yyvsp[(2) - (3)].expr),(yyvsp[(3) - (3)].block));   ;}
    break;

  case 72:
#line 250 "Parser.y"
    {  (yyval.forStmt) = new ast::ForStmt((yyvsp[(2) - (7)].expr),(yyvsp[(4) - (7)].expr),(yyvsp[(6) - (7)].expr),(yyvsp[(7) - (7)].block));   ;}
    break;

  case 73:
#line 251 "Parser.y"
    {  (yyval.forStmt) = new ast::ForStmt((yyvsp[(2) - (6)].varDecl),(yyvsp[(3) - (6)].expr),(yyvsp[(5) - (6)].expr),(yyvsp[(6) - (6)].block));   ;}
    break;

  case 74:
#line 252 "Parser.y"
    {  (yyval.forStmt) = new ast::ForStmt(NULL,(yyvsp[(2) - (3)].expr),NULL,(yyvsp[(3) - (3)].block));   ;}
    break;

  case 75:
#line 253 "Parser.y"
    {  (yyval.forStmt) = new ast::ForStmt(NULL,NULL,NULL,(yyvsp[(2) - (2)].block));   ;}
    break;

  case 76:
#line 256 "Parser.y"
    {  (yyval.switchStmt) = new ast::SwitchStmt((yyvsp[(2) - (5)].expr),(yyvsp[(4) - (5)].caseList));   ;}
    break;

  case 77:
#line 259 "Parser.y"
    {  (yyval.caseList) = (yyvsp[(1) - (2)].caseList); (yyval.caseList)->push_back((yyvsp[(2) - (2)].caseStmt));   ;}
    break;

  case 78:
#line 260 "Parser.y"
    {  (yyval.caseList) = new ast::CaseList();   ;}
    break;

  case 79:
#line 263 "Parser.y"
    {  (yyval.caseStmt) = new ast::CaseStmt((yyvsp[(2) - (4)].expr),(yyvsp[(4) - (4)].stmts));   ;}
    break;

  case 80:
#line 264 "Parser.y"
    {  (yyval.caseStmt) = new ast::CaseStmt(NULL,(yyvsp[(3) - (3)].stmts));   ;}
    break;

  case 81:
#line 268 "Parser.y"
    {  (yyval.continueStmt) = new ast::ContinueStmt();   ;}
    break;

  case 82:
#line 271 "Parser.y"
    {  (yyval.breakStmt) = new ast::BreakStmt();   ;}
    break;

  case 83:
#line 274 "Parser.y"
    {  (yyval.returnStmt) = new ast::ReturnStmt();   ;}
    break;

  case 84:
#line 275 "Parser.y"
    {  (yyval.returnStmt) = new ast::ReturnStmt((yyvsp[(2) - (3)].expr));   ;}
    break;

  case 85:
#line 278 "Parser.y"
    {  (yyval.expr) = new ast::Subscript((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));   ;}
    break;

  case 86:
#line 279 "Parser.y"
    {  (yyval.expr) = new ast::SizeOf(*(yyvsp[(3) - (4)].sVal));   ;}
    break;

  case 87:
#line 280 "Parser.y"
    {  (yyval.expr) = new ast::SizeOf((yyvsp[(3) - (4)].expr));   ;}
    break;

  case 88:
#line 281 "Parser.y"
    {  (yyval.expr) = new ast::SizeOf((yyvsp[(3) - (4)].varType));   ;}
    break;

  case 89:
#line 282 "Parser.y"
    {  (yyval.expr) = new ast::FunctionCall(*(yyvsp[(1) - (4)].sVal),(yyvsp[(3) - (4)].exprList));   ;}
    break;

  case 90:
#line 283 "Parser.y"
    {  (yyval.expr) = new ast::FunctionCall(*(yyvsp[(3) - (6)].sVal),(yyvsp[(5) - (6)].exprList),(yyvsp[(1) - (6)].expr),NULL);   ;}
    break;

  case 91:
#line 284 "Parser.y"
    {  (yyval.expr) = new ast::FunctionCall(*(yyvsp[(3) - (6)].sVal),(yyvsp[(5) - (6)].exprList),NULL,(yyvsp[(1) - (6)].expr));   ;}
    break;

  case 92:
#line 285 "Parser.y"
    {  (yyval.expr) = new ast::StructReference((yyvsp[(1) - (3)].expr),*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 93:
#line 286 "Parser.y"
    {  (yyval.expr) = new ast::StructDereference((yyvsp[(1) - (3)].expr),*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 94:
#line 287 "Parser.y"
    {  (yyval.expr) = new ast::UnaryPlus((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 95:
#line 288 "Parser.y"
    {  (yyval.expr) = new ast::UnaryMinus((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 96:
#line 289 "Parser.y"
    {  (yyval.expr) = new ast::TypeCast((yyvsp[(5) - (6)].varType),(yyvsp[(3) - (6)].expr));   ;}
    break;

  case 97:
#line 290 "Parser.y"
    {  (yyval.expr) = new ast::PrefixInc((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 98:
#line 291 "Parser.y"
    {  (yyval.expr) = new ast::PostfixInc((yyvsp[(1) - (2)].expr));   ;}
    break;

  case 99:
#line 292 "Parser.y"
    {  (yyval.expr) = new ast::PrefixDec((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 100:
#line 293 "Parser.y"
    {  (yyval.expr) = new ast::PostfixDec((yyvsp[(1) - (2)].expr));   ;}
    break;

  case 101:
#line 294 "Parser.y"
    {  (yyval.expr) = new ast::Indirection((yyvsp[(3) - (4)].expr));   ;}
    break;

  case 102:
#line 295 "Parser.y"
    {  (yyval.expr) = new ast::AddressOf((yyvsp[(3) - (4)].expr));   ;}
    break;

  case 103:
#line 296 "Parser.y"
    {  (yyval.expr) = new ast::LogicNot((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 104:
#line 297 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseNot((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 105:
#line 298 "Parser.y"
    {  (yyval.expr) = new ast::Division((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 106:
#line 299 "Parser.y"
    {  (yyval.expr) = new ast::Multiplication((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 107:
#line 300 "Parser.y"
    {  (yyval.expr) = new ast::Modulo((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 108:
#line 301 "Parser.y"
    {  (yyval.expr) = new ast::Addition((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 109:
#line 302 "Parser.y"
    {  (yyval.expr) = new ast::Subtraction((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 110:
#line 303 "Parser.y"
    {  (yyval.expr) = new ast::LeftShift((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 111:
#line 304 "Parser.y"
    {  (yyval.expr) = new ast::RightShift((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 112:
#line 305 "Parser.y"
    {  (yyval.expr) = new ast::LogicGT((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 113:
#line 306 "Parser.y"
    {  (yyval.expr) = new ast::LogicGE((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 114:
#line 307 "Parser.y"
    {  (yyval.expr) = new ast::LogicLT((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 115:
#line 308 "Parser.y"
    {  (yyval.expr) = new ast::LogicLE((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 116:
#line 309 "Parser.y"
    {  (yyval.expr) = new ast::LogicEQ((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 117:
#line 310 "Parser.y"
    {  (yyval.expr) = new ast::LogicNEQ((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 118:
#line 311 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseAND((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 119:
#line 312 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseXOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 120:
#line 313 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 121:
#line 314 "Parser.y"
    {  (yyval.expr) = new ast::LogicAND((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 122:
#line 315 "Parser.y"
    {  (yyval.expr) = new ast::LogicOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 123:
#line 316 "Parser.y"
    {  (yyval.expr) = new ast::TernaryCondition((yyvsp[(1) - (5)].expr),(yyvsp[(3) - (5)].expr),(yyvsp[(5) - (5)].expr));   ;}
    break;

  case 124:
#line 317 "Parser.y"
    {  (yyval.expr) = new ast::DirectAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 125:
#line 318 "Parser.y"
    {  (yyval.expr) = new ast::DivAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 126:
#line 319 "Parser.y"
    {  (yyval.expr) = new ast::MulAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 127:
#line 320 "Parser.y"
    {  (yyval.expr) = new ast::ModAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 128:
#line 321 "Parser.y"
    {  (yyval.expr) = new ast::AddAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 129:
#line 322 "Parser.y"
    {  (yyval.expr) = new ast::SubAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 130:
#line 323 "Parser.y"
    {  (yyval.expr) = new ast::SHLAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 131:
#line 324 "Parser.y"
    {  (yyval.expr) = new ast::SHRAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 132:
#line 325 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseANDAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 133:
#line 326 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseXORAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 134:
#line 327 "Parser.y"
    {  (yyval.expr) = new ast::BitwiseORAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 135:
#line 328 "Parser.y"
    {  (yyval.expr) = (yyvsp[(2) - (3)].expr);   ;}
    break;

  case 136:
#line 329 "Parser.y"
    {  (yyval.expr) = new ast::Variable(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 137:
#line 330 "Parser.y"
    {  (yyval.expr) = (yyvsp[(1) - (1)].constant);   ;}
    break;

  case 138:
#line 331 "Parser.y"
    {  (yyval.expr) = new ast::CommaExpr((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));   ;}
    break;

  case 139:
#line 332 "Parser.y"
    {  (yyval.expr) = new ast::This();   ;}
    break;

  case 140:
#line 335 "Parser.y"
    {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr));   ;}
    break;

  case 141:
#line 336 "Parser.y"
    {  (yyval.exprList) = new ast::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr));   ;}
    break;

  case 142:
#line 337 "Parser.y"
    {  (yyval.exprList) = new ast::ExprList();   ;}
    break;

  case 143:
#line 340 "Parser.y"
    {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr));   ;}
    break;

  case 144:
#line 341 "Parser.y"
    {  (yyval.exprList) = new ast::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr));   ;}
    break;

  case 145:
#line 344 "Parser.y"
    {  (yyval.constant) =  new ast::Constant(true);   ;}
    break;

  case 146:
#line 345 "Parser.y"
    {  (yyval.constant) =  new ast::Constant(false);   ;}
    break;

  case 147:
#line 346 "Parser.y"
    {  (yyval.constant) =  new ast::Constant(0);   ;}
    break;

  case 148:
#line 347 "Parser.y"
    {  (yyval.constant) =  new ast::Constant((yyvsp[(1) - (1)].cVal));   ;}
    break;

  case 149:
#line 348 "Parser.y"
    {  (yyval.constant) =  new ast::Constant((yyvsp[(1) - (1)].iVal));   ;}
    break;

  case 150:
#line 349 "Parser.y"
    {  (yyval.constant) =  new ast::Constant((yyvsp[(1) - (1)].dVal));   ;}
    break;

  case 151:
#line 350 "Parser.y"
    {  (yyval.constant) =  new ast::GlobalString(*(yyvsp[(1) - (1)].strVal));   ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2932 "parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 352 "Parser.y"

