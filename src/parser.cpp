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
  CONST = 309,
  PTR = 310,
  ARRAY = 311,
  DPTR = 312,
  ADDR = 313,
  IF = 314,
  ELSE = 315,
  FOR = 316,
  SWITCH = 317,
  CASE = 318,
  DEFAULT = 319,
  BREAK = 320,
  CONTINUE = 321,
  RETURN = 322,
  SIZEOF = 323,
  TRUE = 324,
  FALSE = 325,
  NULL_ = 326,
  BOOL = 327,
  SHORT = 328,
  INT = 329,
  LONG = 330,
  CHAR = 331,
  FLOAT = 332,
  DOUBLE = 333,
  VOID = 334,
  INTEGER = 335,
  IDENTIFIER = 336,
  REAL = 337,
  CHARACTER = 338,
  STRING = 339,
  FUNC_CALL_ARG_LIST = 340,
  GEQ = 341
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
#define CONST 309
#define PTR 310
#define ARRAY 311
#define DPTR 312
#define ADDR 313
#define IF 314
#define ELSE 315
#define FOR 316
#define SWITCH 317
#define CASE 318
#define DEFAULT 319
#define BREAK 320
#define CONTINUE 321
#define RETURN 322
#define SIZEOF 323
#define TRUE 324
#define FALSE 325
#define NULL_ 326
#define BOOL 327
#define SHORT 328
#define INT 329
#define LONG 330
#define CHAR 331
#define FLOAT 332
#define DOUBLE 333
#define VOID 334
#define INTEGER 335
#define IDENTIFIER 336
#define REAL 337
#define CHARACTER 338
#define STRING 339
#define FUNC_CALL_ARG_LIST 340
#define GEQ 341




/* Copy the first part of user declarations.  */
#line 1 "Parser.y"

#include "ast.hpp" 
#include <string>
#include <iostream>

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

void yyerror(char* s) {
  extern int yylineno;
  std::printf("[Syntax Error]: %s at line %d\n", s, yylineno);
  std::exit(1);
}

int yylex(void);

ast::Program* Root;


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
#line 22 "Parser.y"
{
  int iVal;
  std::string* sVal;
  double dVal;
  char cVal;
  std::string* strVal;
  ast::Program* program;
  ast::Decl* decl;
  ast::Decls* decls;
  ast::FuncDecl* funcDecl;
  ast::FuncBody* funcBody;
  ast::VarDecl* varDecl;
  ast::TypeDecl* typeDecl;
  ast::VarType* varType;
  ast::BuiltInType* builtInType;
  ast::FieldDecls* fieldDecls;
  ast::FieldDecl* fieldDecl;
  ast::MemList* memList;
  ast::Stmt* stmt;
  ast::IfStmt* ifStmt;
  ast::ForStmt* forStmt;
  ast::SwitchStmt* switchStmt;
  ast::CaseList* caseList;
  ast::CaseStmt* caseStmt;
  ast::BreakStmt* breakStmt;
  ast::ContinueStmt* continueStmt;
  ast::ReturnStmt* returnStmt;
  ast::Stmts* stmts;
  ast::Block* block;
  ast::Arg* arg;
  ast::ArgList* argList;
  ast::VarInit* varInit;
  ast::VarList* varList;
  ast::Expr* expr;
  ast::Constant* constant;
  ast::ExprList* exprList;
}
/* Line 193 of yacc.c.  */
#line 325 "parser.cpp"
YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 338 "parser.cpp"

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
YYID(int i)
#else
static int
YYID(i)
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
void* malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free(void*); /* INFRINGES ON USER NAME SPACE */
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
#define YYLAST   1952

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNRULES -- Number of states.  */
#define YYNSTATES  258

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   341

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
      85,    86
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    13,    15,    24,
      33,    37,    41,    45,    47,    48,    52,    54,    56,    60,
      65,    67,    70,    72,    77,    82,    89,    91,    93,    95,
      97,    99,   101,   103,   105,   107,   110,   111,   115,   117,
     121,   123,   124,   128,   130,   134,   138,   140,   142,   143,
     147,   149,   152,   154,   158,   161,   162,   165,   167,   169,
     171,   173,   175,   177,   179,   181,   183,   185,   191,   195,
     203,   210,   214,   217,   223,   226,   227,   232,   236,   239,
     242,   245,   249,   254,   259,   264,   269,   274,   278,   282,
     285,   288,   293,   296,   299,   302,   305,   310,   315,   318,
     321,   325,   329,   333,   337,   341,   345,   349,   353,   357,
     361,   365,   369,   373,   377,   381,   385,   389,   393,   399,
     403,   407,   411,   415,   419,   423,   427,   431,   435,   439,
     443,   447,   449,   451,   455,   459,   461,   462,   466,   468,
     470,   472,   474,   476,   478,   480
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      88,     0,    -1,    89,    -1,    89,    90,    -1,    -1,    91,
      -1,    93,    -1,    97,    -1,    51,    81,    11,   105,    12,
      35,    98,     3,    -1,    51,    81,    11,   105,    12,    35,
      98,    92,    -1,    15,   109,    16,    -1,    98,    94,     3,
      -1,    95,     4,    96,    -1,    96,    -1,    -1,    95,     4,
      96,    -1,    96,    -1,    81,    -1,    81,    28,   119,    -1,
      53,    81,    98,     3,    -1,    99,    -1,    54,    99,    -1,
     100,    -1,    52,    15,   101,    16,    -1,    55,    25,    99,
      23,    -1,    56,    25,    99,     4,    80,    23,    -1,    81,
      -1,    72,    -1,    73,    -1,    74,    -1,    75,    -1,    76,
      -1,    77,    -1,    78,    -1,    79,    -1,   101,   102,    -1,
      -1,    98,   103,     3,    -1,     3,    -1,   104,     4,    81,
      -1,    81,    -1,    -1,   104,     4,    81,    -1,    81,    -1,
     106,     4,   107,    -1,   106,     4,     6,    -1,   107,    -1,
       6,    -1,    -1,   106,     4,   107,    -1,   107,    -1,    98,
      81,    -1,    98,    -1,    15,   109,    16,    -1,   109,   110,
      -1,    -1,   119,     3,    -1,   111,    -1,   112,    -1,   113,
      -1,   117,    -1,   116,    -1,   118,    -1,   108,    -1,    93,
      -1,    97,    -1,     3,    -1,    59,   119,   108,    60,   110,
      -1,    59,   119,   108,    -1,    61,   119,     3,   119,     3,
     119,   108,    -1,    61,    93,   119,     3,   119,   108,    -1,
      61,   119,   108,    -1,    61,   108,    -1,    62,   119,    15,
     114,    16,    -1,   114,   115,    -1,    -1,    63,   119,    10,
     109,    -1,    64,    10,   109,    -1,    66,     3,    -1,    65,
       3,    -1,    67,     3,    -1,    67,   119,     3,    -1,   119,
      13,   119,    14,    -1,    68,    11,    81,    12,    -1,    68,
      11,   119,    12,    -1,    68,    11,    98,    12,    -1,    81,
      11,   120,    12,    -1,   119,     5,    81,    -1,   119,    35,
      81,    -1,    41,   119,    -1,    44,   119,    -1,    11,    98,
      12,   119,    -1,    39,   119,    -1,   119,    39,    -1,    42,
     119,    -1,   119,    42,    -1,    57,    11,   119,    12,    -1,
      58,    11,   119,    12,    -1,    27,   119,    -1,    38,   119,
      -1,   119,    48,   119,    -1,   119,    46,   119,    -1,   119,
      50,   119,    -1,   119,    41,   119,    -1,   119,    44,   119,
      -1,   119,    18,   119,    -1,   119,    20,   119,    -1,   119,
      23,   119,    -1,   119,    86,   119,    -1,   119,    25,   119,
      -1,   119,    24,   119,    -1,   119,    21,   119,    -1,   119,
      26,   119,    -1,   119,    31,   119,    -1,   119,    37,   119,
      -1,   119,    34,   119,    -1,   119,    29,   119,    -1,   119,
      32,   119,    -1,   119,     9,   119,    10,   119,    -1,   119,
      28,   119,    -1,   119,    47,   119,    -1,   119,    45,   119,
      -1,   119,    49,   119,    -1,   119,    40,   119,    -1,   119,
      43,   119,    -1,   119,    17,   119,    -1,   119,    19,   119,
      -1,   119,    30,   119,    -1,   119,    36,   119,    -1,   119,
      33,   119,    -1,    11,   119,    12,    -1,    81,    -1,   122,
      -1,   119,     4,   119,    -1,   121,     4,   119,    -1,   119,
      -1,    -1,   121,     4,   119,    -1,   119,    -1,    69,    -1,
      70,    -1,    71,    -1,    83,    -1,    80,    -1,    82,    -1,
      84,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   131,   131,   134,   135,   138,   139,   140,   143,   144,
     147,   150,   153,   154,   155,   158,   159,   162,   163,   166,
     169,   170,   173,   174,   175,   176,   177,   180,   181,   182,
     183,   184,   185,   186,   187,   190,   191,   194,   195,   198,
     199,   200,   203,   204,   207,   208,   209,   210,   211,   214,
     215,   218,   219,   222,   225,   226,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   242,   243,   246,
     247,   248,   249,   252,   255,   256,   259,   260,   264,   267,
     270,   271,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   328,   329,   330,   333,   334,   337,
     338,   339,   340,   341,   342,   343
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] =
{
  "$end", "error", "$undefined", "SEMI", "COMMA", "DOT", "ELLIPSES",
  "SQUOTE", "DQUOTE", "QUES", "COLON", "LPAREN", "RPAREN", "LBRACKET",
  "RBRACKET", "LBRACE", "RBRACE", "SHLEQ", "SHL", "SHREQ", "SHR", "EQ_",
  "GE", "GT_", "LEQ", "LT_", "NEQ", "NOT", "ASSIGN", "AND", "BANDEQ",
  "BAND", "OR", "BOREQ", "BOR", "ARROW", "BXOREQ", "BXOR", "BNOT", "DADD",
  "ADDEQ", "ADD", "DSUB", "SUBEQ", "SUB", "MULEQ", "MUL", "DIVEQ", "DIV",
  "MODEQ", "MOD", "FUNC", "STRUCT", "TYPEDEF", "CONST", "PTR", "ARRAY",
  "DPTR", "ADDR", "IF", "ELSE", "FOR", "SWITCH", "CASE", "DEFAULT",
  "BREAK", "CONTINUE", "RETURN", "SIZEOF", "TRUE", "FALSE", "NULL_",
  "BOOL", "SHORT", "INT", "LONG", "CHAR", "FLOAT", "DOUBLE", "VOID",
  "INTEGER", "IDENTIFIER", "REAL", "CHARACTER", "STRING",
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
     335,   336,   337,   338,   339,   340,   341
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    89,    89,    90,    90,    90,    91,    91,
      92,    93,    94,    94,    94,    95,    95,    96,    96,    97,
      98,    98,    99,    99,    99,    99,    99,   100,   100,   100,
     100,   100,   100,   100,   100,   101,   101,   102,   102,   103,
     103,   103,   104,   104,   105,   105,   105,   105,   105,   106,
     106,   107,   107,   108,   109,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   111,   111,   112,
     112,   112,   112,   113,   114,   114,   115,   115,   116,   117,
     118,   118,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   120,   120,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     8,     8,
       3,     3,     3,     1,     0,     3,     1,     1,     3,     4,
       1,     2,     1,     4,     4,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     3,     1,     3,
       1,     0,     3,     1,     3,     3,     1,     1,     0,     3,
       1,     2,     1,     3,     2,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     3,     7,
       6,     3,     2,     5,     2,     0,     4,     3,     2,     2,
       2,     3,     4,     4,     4,     4,     4,     3,     3,     2,
       2,     4,     2,     2,     2,     2,     4,     4,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     3,     3,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    26,     3,
       5,     6,     7,    14,    20,    22,     0,    36,     0,    21,
       0,     0,    17,     0,     0,    13,    48,     0,     0,     0,
       0,     0,    11,     0,    47,    52,     0,     0,    46,    38,
      23,    41,    35,    19,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   140,   141,   143,
     131,   144,   142,   145,    18,   132,    12,    51,     0,     0,
      40,     0,     0,     0,   131,     0,     0,    98,    99,    92,
      89,    94,    90,     0,     0,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,    44,    37,     0,    25,     0,
     130,     0,     0,   131,     0,     0,   135,     0,     0,   133,
      87,     0,     0,   125,   105,   126,   106,   111,   107,   110,
     109,   112,   119,   116,   127,   113,   117,   129,   115,    88,
     128,   114,   123,   103,   124,   104,   121,   101,   120,   100,
     122,   102,   108,     0,    39,    91,    96,    97,    83,    85,
      84,    86,     0,     0,    82,     8,    55,     9,   134,   118,
       0,    66,    55,    10,     0,     0,     0,     0,     0,     0,
      64,    65,    63,    54,    57,    58,    59,    61,    60,    62,
       0,     0,     0,     0,    72,     0,     0,    79,    78,    80,
       0,    56,    53,    68,     0,     0,    71,    75,    81,     0,
       0,     0,     0,    67,     0,     0,    73,     0,     0,    74,
      70,     0,     0,    55,    69,    55,    77,    76
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    19,    20,   197,   210,    33,    34,    35,
     211,    23,    24,    25,    37,    52,    81,    82,    46,    47,
      48,   212,   200,   213,   214,   215,   216,   242,   249,   217,
     218,   219,   220,   147,   148,    75
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -160
static const yytype_int16 yypact[] =
{
    -160,     7,  1871,  -160,   -68,     9,   -67,  1823,    -3,     2,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,   -56,  -160,  -160,    15,  -160,  1394,  -160,
    1823,  1823,     5,    25,    26,    31,  1535,  1493,    33,     6,
      35,  1850,  -160,   -56,  -160,   -44,    28,    38,    40,  -160,
    -160,   -35,  -160,  -160,  -160,   -32,  1702,  1850,  1850,  1850,
    1850,  1850,  1850,    46,    65,    71,  -160,  -160,  -160,  -160,
      82,  -160,  -160,  -160,  1211,  -160,    93,  -160,    67,  1575,
     100,   102,   103,    83,    -2,    97,   438,   136,   136,   136,
     136,   136,   136,  1850,  1850,  1776,  1850,  1850,    36,  1850,
    1850,  1850,  1850,  1850,  1850,  1850,  1850,  1850,  1850,  1850,
    1850,  1850,  1850,  1850,  1850,  1850,  1850,    37,  1850,  1850,
    -160,  1850,  1850,  -160,  1850,  1850,  1850,  1850,  1850,  1850,
    1850,  1850,  1850,  1394,  -160,   108,  -160,    42,  -160,  1850,
    -160,   486,   534,    -6,   114,   582,   870,   118,   129,  1211,
    -160,   630,   678,  1211,  1682,  1211,  1682,  1538,   142,   142,
     142,  1538,  1211,  1303,  1211,  1492,  1257,  1211,  1349,  -160,
    1211,  1395,  1211,    -1,  1211,    -1,  1211,   136,  1211,   136,
    1211,   136,   142,     8,   131,   136,  -160,  -160,  -160,  -160,
    -160,  -160,  1850,  1850,  -160,  -160,  -160,  -160,   918,    90,
     967,  -160,  -160,  -160,  1850,  1628,  1850,   134,   139,  1420,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
     246,  1049,   726,  1850,  -160,   198,   774,  -160,  -160,  -160,
     294,  -160,  -160,    79,   342,  1850,  -160,  -160,  -160,  1131,
    1850,   390,   -13,  -160,   726,  1850,  -160,  1850,   140,  -160,
    -160,   726,   822,  -160,  -160,  -160,  1131,  1131
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -160,  -160,  -160,  -160,  -160,     0,  -160,  -160,   101,
     144,    64,     1,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
      74,   -77,  -159,   -85,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,   -41,  -160,  -160,  -160
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -139
static const yytype_int16 yytable[] =
{
      74,   -26,    21,   246,    98,    96,   188,     3,    29,    96,
     -26,   195,   100,    26,    28,    86,    87,    88,    89,    90,
      91,    92,    30,   196,    27,    32,    36,    31,    42,    54,
      43,    39,    40,    41,   117,   -16,    53,    77,   120,    55,
      78,   123,    79,   221,   -50,   127,    80,   129,    83,   131,
     247,   248,   141,   142,   145,   146,   149,    93,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    94,   170,   171,   -26,
     172,   173,    95,   174,   175,   176,   177,   178,   179,   180,
     181,   182,    38,    96,   256,    98,   257,   -15,   185,    99,
      45,    51,   133,   100,   -43,   136,   138,   137,   102,   139,
     104,   105,   -49,   106,   107,   108,   109,   150,   169,   111,
      85,   113,   114,   184,   116,   117,   189,   119,   224,   120,
     191,   122,   123,   192,   125,   -42,   127,   227,   129,   239,
     131,    98,   228,    45,    76,   233,    22,    98,   236,   100,
     253,   198,   199,   135,   243,   100,     0,     0,     0,   144,
     102,     0,   104,   222,   225,   226,     0,   250,   230,     0,
       0,   117,     0,     0,   254,   120,   132,   117,   123,     0,
       0,   120,   234,   122,   123,     0,   125,     0,   127,     0,
     129,     0,   131,     0,   241,     0,     0,   183,     0,   244,
       0,   235,    97,    98,   251,   223,   252,    99,     0,     0,
       0,   100,     0,   202,     0,   101,   102,   103,   104,   105,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   231,
      97,    98,     0,     0,     0,    99,     0,     0,     0,   100,
       0,     0,     0,   101,   102,   103,   104,   105,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   132,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   238,    97,    98,
       0,     0,     0,    99,     0,     0,     0,   100,     0,     0,
       0,   101,   102,   103,   104,   105,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   132,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   240,    97,    98,     0,     0,
       0,    99,     0,     0,     0,   100,     0,     0,     0,   101,
     102,   103,   104,   105,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     132,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   245,    97,    98,     0,     0,     0,    99,
       0,     0,     0,   100,     0,     0,     0,   101,   102,   103,
     104,   105,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   132,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,    97,    98,     0,     0,     0,    99,     0,     0,
     140,   100,     0,     0,     0,   101,   102,   103,   104,   105,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   132,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
      97,    98,     0,     0,     0,    99,     0,     0,   186,   100,
       0,     0,     0,   101,   102,   103,   104,   105,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   132,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,    97,    98,
       0,     0,     0,    99,     0,     0,   187,   100,     0,     0,
       0,   101,   102,   103,   104,   105,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   132,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,    97,    98,     0,     0,
       0,    99,     0,     0,   190,   100,     0,     0,     0,   101,
     102,   103,   104,   105,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     132,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,    97,    98,     0,     0,     0,    99,
     193,     0,     0,   100,     0,     0,     0,   101,   102,   103,
     104,   105,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   132,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,    97,    98,     0,     0,     0,    99,     0,     0,
       0,   100,   194,     0,     0,   101,   102,   103,   104,   105,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   132,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
      97,    98,     0,     0,     0,    99,     0,     0,     0,   100,
       0,   202,     0,   101,   102,   103,   104,   105,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   132,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,    97,    98,
       0,     0,     0,    99,     0,     0,     0,   100,     0,   237,
       0,   101,   102,   103,   104,   105,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   132,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,    97,    98,     0,     0,
       0,    99,   255,     0,     0,   100,     0,     0,     0,   101,
     102,   103,   104,   105,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     132,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,  -138,    98,     0,     0,     0,    99,
       0,     0,     0,   100,     0,     0,     0,   101,   102,   103,
     104,   105,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   132,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,  -137,    98,     0,     0,     0,    99,     0,     0,
       0,   100,     0,     0,     0,   101,   102,   103,   104,   105,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   132,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     201,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,   202,   203,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   132,    58,    59,     0,    60,    61,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    63,    64,   204,     0,   205,   206,
       0,     0,   207,   208,   209,    65,    66,    67,    68,    10,
      11,    12,    13,    14,    15,    16,    17,    69,    84,    71,
      72,    73,   201,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,   202,   232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,     0,
      60,    61,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    63,    64,   204,     0,
     205,   206,     0,     0,   207,   208,   209,    65,    66,    67,
      68,    10,    11,    12,    13,    14,    15,    16,    17,    69,
      84,    71,    72,    73,   201,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,   202,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,     0,    60,    61,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     8,     9,    63,    64,
     204,     0,   205,   206,     0,     0,   207,   208,   209,    65,
      66,    67,    68,    10,    11,    12,    13,    14,    15,    16,
      17,    69,    84,    71,    72,    73,    98,     0,     0,     0,
      99,     0,     0,     0,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,     0,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    98,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,     0,     0,   102,     0,   104,   105,     0,
     106,   107,   108,   109,     0,     0,   111,     0,   113,     0,
       0,   116,   117,     0,   119,     0,   120,   132,   122,   123,
       0,   125,     0,   127,     0,   129,     0,   131,    98,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,   102,     0,   104,   105,     0,   106,   107,   108,   109,
       0,     0,     0,     0,   113,     0,     0,   116,   117,     0,
     119,     0,   120,   132,   122,   123,     0,   125,     0,   127,
       0,   129,     0,   131,    98,     0,     0,     0,     0,     0,
       0,     0,   100,     0,     0,     0,     0,   102,     0,   104,
     105,     0,   106,   107,   108,   109,     0,     0,     0,     0,
     113,     0,     0,     0,   117,     0,   119,     0,   120,   132,
     122,   123,     0,   125,     0,   127,     0,   129,     0,   131,
      98,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,     0,     0,   102,     0,   104,   105,     0,   106,   107,
     108,   109,     0,   229,     0,     0,   113,     0,     0,     0,
     117,    56,     0,     0,   120,   132,   122,   123,     0,   125,
       0,   127,     0,   129,     0,   131,     5,    57,     7,     8,
       9,     0,     0,     0,     0,     0,     0,     0,    58,    59,
       0,    60,    61,     0,    62,     0,    10,    11,    12,    13,
      14,    15,    16,    17,     0,    18,     0,    63,    64,     0,
       0,   132,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,     0,     0,     0,     0,    49,    98,     0,     0,
      69,    70,    71,    72,    73,   100,     0,     0,     0,    50,
     102,     0,   104,   105,     0,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,   117,     0,     0,
       0,   120,     0,   122,   123,     0,   125,     0,   127,     0,
     129,    44,   131,    98,     0,     5,     0,     7,     8,     9,
       0,   100,     0,     0,     0,     0,   102,     0,   104,     0,
       0,   106,   107,   108,     0,    10,    11,    12,    13,    14,
      15,    16,    17,   117,    18,     0,     0,   120,   132,   122,
     123,   134,   125,     0,   127,     0,   129,     5,   131,     7,
       8,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,     0,     0,     0,
       0,     0,     0,     0,   132,     0,     0,     5,     0,     7,
       8,     9,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,   202,     0,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    57,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,     0,    60,
      61,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       5,     0,     7,     8,     9,    63,    64,    98,     0,     0,
       0,     0,     0,     0,     0,   100,    65,    66,    67,    68,
      10,    11,    12,    13,    14,    15,    16,    17,    69,    84,
      71,    72,    73,    56,     0,     0,     0,   117,     0,     0,
       0,   120,     0,   122,   123,     0,   125,     0,   127,    57,
     129,     0,   131,     0,     0,     0,     0,     0,     0,     0,
      58,    59,     0,    60,    61,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     7,     8,     9,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    10,    11,    12,    13,    14,    15,
      16,    17,    69,    84,    71,    72,    73,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,     0,    60,    61,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     5,     0,
       7,     8,     9,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    10,    11,
      12,    13,    14,    15,    16,    17,    69,   143,    71,    72,
      73,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,    57,     8,     9,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
       0,    60,    61,     0,    62,    10,    11,    12,    13,    14,
      15,    16,    17,     0,    18,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,     4,     5,     6,     7,     8,     9,     0,     0,
      69,    70,    71,    72,    73,     0,     0,     0,     0,     0,
       0,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,     0,    18
};

static const yytype_int16 yycheck[] =
{
      41,     3,     2,    16,     5,    11,    12,     0,     7,    11,
      12,     3,    13,    81,    81,    56,    57,    58,    59,    60,
      61,    62,    25,    15,    15,    81,    11,    25,     3,    23,
       4,    30,    31,    28,    35,     4,     3,    81,    39,     4,
      12,    42,     4,   202,     4,    46,    81,    48,    80,    50,
      63,    64,    93,    94,    95,    96,    97,    11,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    11,   118,   119,    81,
     121,   122,    11,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    28,    11,   253,     5,   255,     4,   139,     9,
      36,    37,    35,    13,     4,     3,    23,     4,    18,    12,
      20,    21,     4,    23,    24,    25,    26,    81,    81,    29,
      56,    31,    32,    81,    34,    35,    12,    37,   205,    39,
      12,    41,    42,     4,    44,     4,    46,     3,    48,    60,
      50,     5,     3,    79,    43,   222,     2,     5,   225,    13,
      10,   192,   193,    79,   239,    13,    -1,    -1,    -1,    95,
      18,    -1,    20,   204,   205,   206,    -1,   244,   209,    -1,
      -1,    35,    -1,    -1,   251,    39,    86,    35,    42,    -1,
      -1,    39,   223,    41,    42,    -1,    44,    -1,    46,    -1,
      48,    -1,    50,    -1,   235,    -1,    -1,   133,    -1,   240,
      -1,     3,     4,     5,   245,   205,   247,     9,    -1,    -1,
      -1,    13,    -1,    15,    -1,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     3,
       4,     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    86,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     3,     4,     5,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    86,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     3,     4,     5,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      86,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,     3,     4,     5,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    86,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,     4,     5,    -1,    -1,    -1,     9,    -1,    -1,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    86,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
       4,     5,    -1,    -1,    -1,     9,    -1,    -1,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    86,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,     4,     5,
      -1,    -1,    -1,     9,    -1,    -1,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    86,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,     4,     5,    -1,    -1,
      -1,     9,    -1,    -1,    12,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      86,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,     4,     5,    -1,    -1,    -1,     9,
      10,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    86,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,     4,     5,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    86,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
       4,     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    15,    -1,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    86,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,     4,     5,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    15,
      -1,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    86,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,     4,     5,    -1,    -1,
      -1,     9,    10,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      86,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,     4,     5,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    86,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,     4,     5,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    86,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      -1,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    -1,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,     5,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    31,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    86,    41,    42,
      -1,    44,    -1,    46,    -1,    48,    -1,    50,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    23,    24,    25,    26,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,    -1,
      37,    -1,    39,    86,    41,    42,    -1,    44,    -1,    46,
      -1,    48,    -1,    50,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,
      21,    -1,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,    86,
      41,    42,    -1,    44,    -1,    46,    -1,    48,    -1,    50,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    18,    -1,    20,    21,    -1,    23,    24,
      25,    26,    -1,     3,    -1,    -1,    31,    -1,    -1,    -1,
      35,    11,    -1,    -1,    39,    86,    41,    42,    -1,    44,
      -1,    46,    -1,    48,    -1,    50,    52,    27,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    -1,    57,    58,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,     3,     5,    -1,    -1,
      80,    81,    82,    83,    84,    13,    -1,    -1,    -1,    16,
      18,    -1,    20,    21,    -1,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    42,    -1,    44,    -1,    46,    -1,
      48,     6,    50,     5,    -1,    52,    -1,    54,    55,    56,
      -1,    13,    -1,    -1,    -1,    -1,    18,    -1,    20,    -1,
      -1,    23,    24,    25,    -1,    72,    73,    74,    75,    76,
      77,    78,    79,    35,    81,    -1,    -1,    39,    86,    41,
      42,     6,    44,    -1,    46,    -1,    48,    52,    50,    54,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    77,    78,    79,    27,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    56,    57,    58,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    11,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    42,    -1,    44,    -1,    46,    27,
      48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    54,    55,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    27,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    42,    -1,    44,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    -1,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    51,    52,    53,    54,    55,    56,    -1,    -1,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    88,    89,     0,    51,    52,    53,    54,    55,    56,
      72,    73,    74,    75,    76,    77,    78,    79,    81,    90,
      91,    93,    97,    98,    99,   100,    81,    15,    81,    99,
      25,    25,    81,    94,    95,    96,    11,   101,    98,    99,
      99,    28,     3,     4,     6,    98,   105,   106,   107,     3,
      16,    98,   102,     3,    23,     4,    11,    27,    38,    39,
      41,    42,    44,    57,    58,    68,    69,    70,    71,    80,
      81,    82,    83,    84,   119,   122,    96,    81,    12,     4,
      81,   103,   104,    80,    81,    98,   119,   119,   119,   119,
     119,   119,   119,    11,    11,    11,    11,     4,     5,     9,
      13,    17,    18,    19,    20,    21,    23,    24,    25,    26,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    86,    35,     6,   107,     3,     4,    23,    12,
      12,   119,   119,    81,    98,   119,   119,   120,   121,   119,
      81,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,    81,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,    98,    81,   119,    12,    12,    12,    12,
      12,    12,     4,    10,    14,     3,    15,    92,   119,   119,
     109,     3,    15,    16,    59,    61,    62,    65,    66,    67,
      93,    97,   108,   110,   111,   112,   113,   116,   117,   118,
     119,   109,   119,    93,   108,   119,   119,     3,     3,     3,
     119,     3,    16,   108,   119,     3,   108,    15,     3,    60,
       3,   119,   114,   110,   119,     3,    16,    63,    64,   115,
     108,   119,   119,    10,   108,    10,   109,   109
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
yy_symbol_value_print(FILE* yyoutput, int yytype, YYSTYPE const* const yyvaluep)
#else
static void
yy_symbol_value_print(yyoutput, yytype, yyvaluep)
FILE* yyoutput;
int yytype;
YYSTYPE const* const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE(yyoutput);
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
yy_symbol_print(FILE* yyoutput, int yytype, YYSTYPE const* const yyvaluep)
#else
static void
yy_symbol_print(yyoutput, yytype, yyvaluep)
FILE* yyoutput;
int yytype;
YYSTYPE const* const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF(yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF(yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print(yyoutput, yytype, yyvaluep);
  YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print(yytype_int16* bottom, yytype_int16* top)
#else
static void
yy_stack_print(bottom, top)
yytype_int16* bottom;
yytype_int16* top;
#endif
{
  YYFPRINTF(stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF(stderr, " %d", *bottom);
  YYFPRINTF(stderr, "\n");
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
yy_reduce_print(YYSTYPE* yyvsp, int yyrule)
#else
static void
yy_reduce_print(yyvsp, yyrule)
YYSTYPE* yyvsp;
int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n",
    yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
  {
    fprintf(stderr, "   $%d = ", yyi + 1);
    yy_symbol_print(stderr, yyrhs[yyprhs[yyrule] + yyi],
      &(yyvsp[(yyi + 1) - (yynrhs)])
    );
    fprintf(stderr, "\n");
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
yystrlen(const char* yystr)
#else
static YYSIZE_T
yystrlen(yystr)
const char* yystr;
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
static char*
yystpcpy(char* yydest, const char* yysrc)
#else
static char*
yystpcpy(yydest, yysrc)
char* yydest;
const char* yysrc;
#endif
{
  char* yyd = yydest;
  const char* yys = yysrc;

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
yytnamerr(char* yyres, const char* yystr)
{
  if (*yystr == '"')
  {
    YYSIZE_T yyn = 0;
    char const* yyp = yystr;

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
  do_not_strip_quotes:;
  }

  if (!yyres)
    return yystrlen(yystr);

  return yystpcpy(yyres, yystr) - yyres;
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
yysyntax_error(char* yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (!(YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
  {
    int yytype = YYTRANSLATE(yychar);
    YYSIZE_T yysize0 = yytnamerr(0, yytname[yytype]);
    YYSIZE_T yysize = yysize0;
    YYSIZE_T yysize1;
    int yysize_overflow = 0;
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    char const* yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
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
    char* yyfmt;
    char const* yyf;
    static char const yyunexpected[] = "syntax error, unexpected %s";
    static char const yyexpecting[] = ", expecting %s";
    static char const yyor[] = " or %s";
    char yyformat[sizeof yyunexpected
      + sizeof yyexpecting - 1
      + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
        * (sizeof yyor - 1))];
    char const* yyprefix = yyexpecting;

    /* Start YYX at -YYN if negative to avoid negative indexes in
 YYCHECK.  */
    int yyxbegin = yyn < 0 ? -yyn : 0;

    /* Stay within bounds of both yycheck and yytname.  */
    int yychecklim = YYLAST - yyn + 1;
    int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
    int yycount = 1;

    yyarg[0] = yytname[yytype];
    yyfmt = yystpcpy(yyformat, yyunexpected);

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
        yysize1 = yysize + yytnamerr(0, yytname[yyx]);
        yysize_overflow |= (yysize1 < yysize);
        yysize = yysize1;
        yyfmt = yystpcpy(yyfmt, yyprefix);
        yyprefix = yyor;
      }

    yyf = YY_(yyformat);
    yysize1 = yysize + yystrlen(yyf);
    yysize_overflow |= (yysize1 < yysize);
    yysize = yysize1;

    if (yysize_overflow)
      return YYSIZE_MAXIMUM;

    if (yyresult)
    {
      /* Avoid sprintf, as that infringes on the user's name space.
         Don't have undefined behavior even if the translation
         produced a string with the wrong number of "%s"s.  */
      char* yyp = yyresult;
      int yyi = 0;
      while ((*yyp = *yyf) != '\0')
      {
        if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr(yyp, yyarg[yyi++]);
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
yydestruct(const char* yymsg, int yytype, YYSTYPE* yyvaluep)
#else
static void
yydestruct(yymsg, yytype, yyvaluep)
const char* yymsg;
int yytype;
YYSTYPE* yyvaluep;
#endif
{
  YYUSE(yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
  {

  default:
    break;
  }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse(void* YYPARSE_PARAM);
#else
int yyparse();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse(void);
#else
int yyparse();
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
yyparse(void* YYPARSE_PARAM)
#else
int
yyparse(YYPARSE_PARAM)
void* YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse(void)
#else
int
yyparse()

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
  char* yymsg = yymsgbuf;
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
  yytype_int16* yyss = yyssa;
  yytype_int16* yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE* yyvs = yyvsa;
  YYSTYPE* yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF((stderr, "Starting parse\n"));

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
      YYSTYPE* yyvs1 = yyvs;
      yytype_int16* yyss1 = yyss;


      /* Each stack pointer address is followed by the size of the
         data in use in that stack, in bytes.  This used to be a
         conditional around just the two extra args, but that might
         be undefined if yyoverflow is a macro.  */
      yyoverflow(YY_("memory exhausted"),
        &yyss1, yysize * sizeof(*yyssp),
        &yyvs1, yysize * sizeof(*yyvsp),

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
      yytype_int16* yyss1 = yyss;
      union yyalloc* yyptr =
        (union yyalloc*)YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
      if (!yyptr)
        goto yyexhaustedlab;
      YYSTACK_RELOCATE(yyss);
      YYSTACK_RELOCATE(yyvs);

#  undef YYSTACK_RELOCATE
      if (yyss1 != yyssa)
        YYSTACK_FREE(yyss1);
    }
# endif
#endif /* no yyoverflow */

    yyssp = yyss + yysize - 1;
    yyvsp = yyvs + yysize - 1;


    YYDPRINTF((stderr, "Stack size increased to %lu\n",
      (unsigned long int) yystacksize));

    if (yyss + yystacksize - 1 <= yyssp)
      YYABORT;
  }

  YYDPRINTF((stderr, "Entering state %d\n", yystate));

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
    YYDPRINTF((stderr, "Reading a token: "));
    yychar = YYLEX;
  }

  if (yychar <= YYEOF)
  {
    yychar = yytoken = YYEOF;
    YYDPRINTF((stderr, "Now at end of input.\n"));
  }
  else
  {
    yytoken = YYTRANSLATE(yychar);
    YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
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
  YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

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
  yyval = yyvsp[1 - yylen];


  YY_REDUCE_PRINT(yyn);
  switch (yyn)
  {
  case 2:
#line 131 "Parser.y"
  {  (yyval.program) = new ast::Program((yyvsp[(1) - (1)].decls)); Root = (yyval.program); ;}
  break;

  case 3:
#line 134 "Parser.y"
  {  (yyval.decls) = (yyvsp[(1) - (2)].decls); (yyval.decls)->push_back((yyvsp[(2) - (2)].decl)); ;}
  break;

  case 4:
#line 135 "Parser.y"
  {  (yyval.decls) = new ast::Decls(); ;}
  break;

  case 5:
#line 138 "Parser.y"
  {  (yyval.decl) = (yyvsp[(1) - (1)].funcDecl); ;}
  break;

  case 6:
#line 139 "Parser.y"
  {  (yyval.decl) = (yyvsp[(1) - (1)].varDecl); ;}
  break;

  case 7:
#line 140 "Parser.y"
  {  (yyval.decl) = (yyvsp[(1) - (1)].typeDecl); ;}
  break;

  case 8:
#line 143 "Parser.y"
  {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(7) - (8)].varType), *(yyvsp[(2) - (8)].sVal), (yyvsp[(4) - (8)].argList)); ;}
  break;

  case 9:
#line 144 "Parser.y"
  {  (yyval.funcDecl) = new ast::FuncDecl((yyvsp[(7) - (8)].varType), *(yyvsp[(2) - (8)].sVal), (yyvsp[(4) - (8)].argList), (yyvsp[(8) - (8)].funcBody)); ;}
  break;

  case 10:
#line 147 "Parser.y"
  {  (yyval.funcBody) = new ast::FuncBody((yyvsp[(2) - (3)].stmts)); ;}
  break;

  case 11:
#line 150 "Parser.y"
  {  (yyval.varDecl) = new ast::VarDecl((yyvsp[(1) - (3)].varType), (yyvsp[(2) - (3)].varList)); ;}
  break;

  case 12:
#line 153 "Parser.y"
  {  (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit)); ;}
  break;

  case 13:
#line 154 "Parser.y"
  {  (yyval.varList) = new ast::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit)); ;}
  break;

  case 14:
#line 155 "Parser.y"
  {  (yyval.varList) = new ast::VarList(); ;}
  break;

  case 15:
#line 158 "Parser.y"
  {  (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit)); ;}
  break;

  case 16:
#line 159 "Parser.y"
  {  (yyval.varList) = new ast::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit)); ;}
  break;

  case 17:
#line 162 "Parser.y"
  {  (yyval.varInit) = new ast::VarInit(*(yyvsp[(1) - (1)].sVal)); ;}
  break;

  case 18:
#line 163 "Parser.y"
  {  (yyval.varInit) = new ast::VarInit(*(yyvsp[(1) - (3)].sVal), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 19:
#line 166 "Parser.y"
  {  (yyval.typeDecl) = new ast::TypeDecl(*(yyvsp[(2) - (4)].sVal), (yyvsp[(3) - (4)].varType)); ;}
  break;

  case 20:
#line 169 "Parser.y"
  {  (yyval.varType) = (yyvsp[(1) - (1)].varType); ;}
  break;

  case 21:
#line 170 "Parser.y"
  {  (yyval.varType) = (yyvsp[(2) - (2)].varType); (yyval.varType)->SetConst(); ;}
  break;

  case 22:
#line 173 "Parser.y"
  {  (yyval.varType) = (yyvsp[(1) - (1)].builtInType); ;}
  break;

  case 23:
#line 174 "Parser.y"
  {  (yyval.varType) = new ast::StructType((yyvsp[(3) - (4)].fieldDecls)); ;}
  break;

  case 24:
#line 175 "Parser.y"
  {  (yyval.varType) = new ast::PointerType((yyvsp[(3) - (4)].varType)); ;}
  break;

  case 25:
#line 176 "Parser.y"
  {  (yyval.varType) = new ast::ArrayType((yyvsp[(3) - (6)].varType), (yyvsp[(5) - (6)].iVal)); ;}
  break;

  case 26:
#line 177 "Parser.y"
  {  (yyval.varType) = new ast::DefinedType(*(yyvsp[(1) - (1)].sVal)); ;}
  break;

  case 27:
#line 180 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Bool); ;}
  break;

  case 28:
#line 181 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Short); ;}
  break;

  case 29:
#line 182 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Int); ;}
  break;

  case 30:
#line 183 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Long); ;}
  break;

  case 31:
#line 184 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Char); ;}
  break;

  case 32:
#line 185 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Float); ;}
  break;

  case 33:
#line 186 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Double); ;}
  break;

  case 34:
#line 187 "Parser.y"
  {  (yyval.builtInType) = new ast::BuiltInType(ast::BuiltInType::TypeID::_Void); ;}
  break;

  case 35:
#line 190 "Parser.y"
  {  (yyval.fieldDecls) = (yyvsp[(1) - (2)].fieldDecls); if ((yyvsp[(2) - (2)].fieldDecl) != NULL) (yyval.fieldDecls)->push_back((yyvsp[(2) - (2)].fieldDecl)); ;}
  break;

  case 36:
#line 191 "Parser.y"
  {  (yyval.fieldDecls) = new ast::FieldDecls(); ;}
  break;

  case 37:
#line 194 "Parser.y"
  {  (yyval.fieldDecl) = new ast::FieldDecl((yyvsp[(1) - (3)].varType), (yyvsp[(2) - (3)].memList)); ;}
  break;

  case 38:
#line 195 "Parser.y"
  {  (yyval.fieldDecl) = NULL; ;}
  break;

  case 39:
#line 198 "Parser.y"
  {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal)); ;}
  break;

  case 40:
#line 199 "Parser.y"
  {  (yyval.memList) = new ast::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal)); ;}
  break;

  case 41:
#line 200 "Parser.y"
  {  (yyval.memList) = new ast::MemList(); ;}
  break;

  case 42:
#line 203 "Parser.y"
  {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal)); ;}
  break;

  case 43:
#line 204 "Parser.y"
  {  (yyval.memList) = new ast::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal)); ;}
  break;

  case 44:
#line 207 "Parser.y"
  {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg)); ;}
  break;

  case 45:
#line 208 "Parser.y"
  {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->SetVarArg(); ;}
  break;

  case 46:
#line 209 "Parser.y"
  {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg)); ;}
  break;

  case 47:
#line 210 "Parser.y"
  {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->SetVarArg(); ;}
  break;

  case 48:
#line 211 "Parser.y"
  {  (yyval.argList) = new ast::ArgList(); ;}
  break;

  case 49:
#line 214 "Parser.y"
  {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg)); ;}
  break;

  case 50:
#line 215 "Parser.y"
  {  (yyval.argList) = new ast::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg)); ;}
  break;

  case 51:
#line 218 "Parser.y"
  {  (yyval.arg) = new ast::Arg((yyvsp[(1) - (2)].varType), *(yyvsp[(2) - (2)].sVal)); ;}
  break;

  case 52:
#line 219 "Parser.y"
  {  (yyval.arg) = new ast::Arg((yyvsp[(1) - (1)].varType)); ;}
  break;

  case 53:
#line 222 "Parser.y"
  {  (yyval.block) = new ast::Block((yyvsp[(2) - (3)].stmts)); ;}
  break;

  case 54:
#line 225 "Parser.y"
  {  (yyval.stmts) = (yyvsp[(1) - (2)].stmts); if ((yyvsp[(2) - (2)].stmt) != NULL) (yyval.stmts)->push_back((yyvsp[(2) - (2)].stmt)); ;}
  break;

  case 55:
#line 226 "Parser.y"
  {  (yyval.stmts) = new ast::Stmts(); ;}
  break;

  case 56:
#line 229 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (2)].expr); ;}
  break;

  case 57:
#line 230 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].ifStmt); ;}
  break;

  case 58:
#line 231 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].forStmt); ;}
  break;

  case 59:
#line 232 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].switchStmt); ;}
  break;

  case 60:
#line 233 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].breakStmt); ;}
  break;

  case 61:
#line 234 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].continueStmt); ;}
  break;

  case 62:
#line 235 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].returnStmt); ;}
  break;

  case 63:
#line 236 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].block); ;}
  break;

  case 64:
#line 237 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].varDecl); ;}
  break;

  case 65:
#line 238 "Parser.y"
  {  (yyval.stmt) = (yyvsp[(1) - (1)].typeDecl); ;}
  break;

  case 66:
#line 239 "Parser.y"
  {  (yyval.stmt) = NULL; ;}
  break;

  case 67:
#line 242 "Parser.y"
  {  (yyval.ifStmt) = new ast::IfStmt((yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].block), (yyvsp[(5) - (5)].stmt)); ;}
  break;

  case 68:
#line 243 "Parser.y"
  {  (yyval.ifStmt) = new ast::IfStmt((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].block)); ;}
  break;

  case 69:
#line 246 "Parser.y"
  {  (yyval.forStmt) = new ast::ForStmt((yyvsp[(2) - (7)].expr), (yyvsp[(4) - (7)].expr), (yyvsp[(6) - (7)].expr), (yyvsp[(7) - (7)].block)); ;}
  break;

  case 70:
#line 247 "Parser.y"
  {  (yyval.forStmt) = new ast::ForStmt((yyvsp[(2) - (6)].varDecl), (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].expr), (yyvsp[(6) - (6)].block)); ;}
  break;

  case 71:
#line 248 "Parser.y"
  {  (yyval.forStmt) = new ast::ForStmt(NULL, (yyvsp[(2) - (3)].expr), NULL, (yyvsp[(3) - (3)].block)); ;}
  break;

  case 72:
#line 249 "Parser.y"
  {  (yyval.forStmt) = new ast::ForStmt(NULL, NULL, NULL, (yyvsp[(2) - (2)].block)); ;}
  break;

  case 73:
#line 252 "Parser.y"
  {  (yyval.switchStmt) = new ast::SwitchStmt((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].caseList)); ;}
  break;

  case 74:
#line 255 "Parser.y"
  {  (yyval.caseList) = (yyvsp[(1) - (2)].caseList); (yyval.caseList)->push_back((yyvsp[(2) - (2)].caseStmt)); ;}
  break;

  case 75:
#line 256 "Parser.y"
  {  (yyval.caseList) = new ast::CaseList(); ;}
  break;

  case 76:
#line 259 "Parser.y"
  {  (yyval.caseStmt) = new ast::CaseStmt((yyvsp[(2) - (4)].expr), (yyvsp[(4) - (4)].stmts)); ;}
  break;

  case 77:
#line 260 "Parser.y"
  {  (yyval.caseStmt) = new ast::CaseStmt(NULL, (yyvsp[(3) - (3)].stmts)); ;}
  break;

  case 78:
#line 264 "Parser.y"
  {  (yyval.continueStmt) = new ast::ContinueStmt(); ;}
  break;

  case 79:
#line 267 "Parser.y"
  {  (yyval.breakStmt) = new ast::BreakStmt(); ;}
  break;

  case 80:
#line 270 "Parser.y"
  {  (yyval.returnStmt) = new ast::ReturnStmt(); ;}
  break;

  case 81:
#line 271 "Parser.y"
  {  (yyval.returnStmt) = new ast::ReturnStmt((yyvsp[(2) - (3)].expr)); ;}
  break;

  case 82:
#line 274 "Parser.y"
  {  (yyval.expr) = new ast::Subscript((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr)); ;}
  break;

  case 83:
#line 275 "Parser.y"
  {  (yyval.expr) = new ast::SizeOf(*(yyvsp[(3) - (4)].sVal)); ;}
  break;

  case 84:
#line 276 "Parser.y"
  {  (yyval.expr) = new ast::SizeOf((yyvsp[(3) - (4)].expr)); ;}
  break;

  case 85:
#line 277 "Parser.y"
  {  (yyval.expr) = new ast::SizeOf((yyvsp[(3) - (4)].varType)); ;}
  break;

  case 86:
#line 278 "Parser.y"
  {  (yyval.expr) = new ast::FunctionCall(*(yyvsp[(1) - (4)].sVal), (yyvsp[(3) - (4)].exprList)); ;}
  break;

  case 87:
#line 279 "Parser.y"
  {  (yyval.expr) = new ast::StructReference((yyvsp[(1) - (3)].expr), *(yyvsp[(3) - (3)].sVal)); ;}
  break;

  case 88:
#line 280 "Parser.y"
  {  (yyval.expr) = new ast::StructDereference((yyvsp[(1) - (3)].expr), *(yyvsp[(3) - (3)].sVal)); ;}
  break;

  case 89:
#line 281 "Parser.y"
  {  (yyval.expr) = new ast::UnaryPlus((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 90:
#line 282 "Parser.y"
  {  (yyval.expr) = new ast::UnaryMinus((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 91:
#line 283 "Parser.y"
  {  (yyval.expr) = new ast::TypeCast((yyvsp[(2) - (4)].varType), (yyvsp[(4) - (4)].expr)); ;}
  break;

  case 92:
#line 284 "Parser.y"
  {  (yyval.expr) = new ast::PrefixInc((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 93:
#line 285 "Parser.y"
  {  (yyval.expr) = new ast::PostfixInc((yyvsp[(1) - (2)].expr)); ;}
  break;

  case 94:
#line 286 "Parser.y"
  {  (yyval.expr) = new ast::PrefixDec((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 95:
#line 287 "Parser.y"
  {  (yyval.expr) = new ast::PostfixDec((yyvsp[(1) - (2)].expr)); ;}
  break;

  case 96:
#line 288 "Parser.y"
  {  (yyval.expr) = new ast::Indirection((yyvsp[(3) - (4)].expr)); ;}
  break;

  case 97:
#line 289 "Parser.y"
  {  (yyval.expr) = new ast::AddressOf((yyvsp[(3) - (4)].expr)); ;}
  break;

  case 98:
#line 290 "Parser.y"
  {  (yyval.expr) = new ast::LogicNot((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 99:
#line 291 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseNot((yyvsp[(2) - (2)].expr)); ;}
  break;

  case 100:
#line 292 "Parser.y"
  {  (yyval.expr) = new ast::Division((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 101:
#line 293 "Parser.y"
  {  (yyval.expr) = new ast::Multiplication((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 102:
#line 294 "Parser.y"
  {  (yyval.expr) = new ast::Modulo((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 103:
#line 295 "Parser.y"
  {  (yyval.expr) = new ast::Addition((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 104:
#line 296 "Parser.y"
  {  (yyval.expr) = new ast::Subtraction((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 105:
#line 297 "Parser.y"
  {  (yyval.expr) = new ast::LeftShift((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 106:
#line 298 "Parser.y"
  {  (yyval.expr) = new ast::RightShift((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 107:
#line 299 "Parser.y"
  {  (yyval.expr) = new ast::LogicGT((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 108:
#line 300 "Parser.y"
  {  (yyval.expr) = new ast::LogicGE((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 109:
#line 301 "Parser.y"
  {  (yyval.expr) = new ast::LogicLT((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 110:
#line 302 "Parser.y"
  {  (yyval.expr) = new ast::LogicLE((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 111:
#line 303 "Parser.y"
  {  (yyval.expr) = new ast::LogicEQ((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 112:
#line 304 "Parser.y"
  {  (yyval.expr) = new ast::LogicNEQ((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 113:
#line 305 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseAND((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 114:
#line 306 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseXOR((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 115:
#line 307 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseOR((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 116:
#line 308 "Parser.y"
  {  (yyval.expr) = new ast::LogicAND((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 117:
#line 309 "Parser.y"
  {  (yyval.expr) = new ast::LogicOR((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 118:
#line 310 "Parser.y"
  {  (yyval.expr) = new ast::TernaryCondition((yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].expr)); ;}
  break;

  case 119:
#line 311 "Parser.y"
  {  (yyval.expr) = new ast::DirectAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 120:
#line 312 "Parser.y"
  {  (yyval.expr) = new ast::DivAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 121:
#line 313 "Parser.y"
  {  (yyval.expr) = new ast::MulAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 122:
#line 314 "Parser.y"
  {  (yyval.expr) = new ast::ModAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 123:
#line 315 "Parser.y"
  {  (yyval.expr) = new ast::AddAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 124:
#line 316 "Parser.y"
  {  (yyval.expr) = new ast::SubAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 125:
#line 317 "Parser.y"
  {  (yyval.expr) = new ast::SHLAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 126:
#line 318 "Parser.y"
  {  (yyval.expr) = new ast::SHRAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 127:
#line 319 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseANDAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 128:
#line 320 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseXORAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 129:
#line 321 "Parser.y"
  {  (yyval.expr) = new ast::BitwiseORAssign((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 130:
#line 322 "Parser.y"
  {  (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
  break;

  case 131:
#line 323 "Parser.y"
  {  (yyval.expr) = new ast::Variable(*(yyvsp[(1) - (1)].sVal)); ;}
  break;

  case 132:
#line 324 "Parser.y"
  {  (yyval.expr) = (yyvsp[(1) - (1)].constant); ;}
  break;

  case 133:
#line 325 "Parser.y"
  {  (yyval.expr) = new ast::CommaExpr((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
  break;

  case 134:
#line 328 "Parser.y"
  {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr)); ;}
  break;

  case 135:
#line 329 "Parser.y"
  {  (yyval.exprList) = new ast::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr)); ;}
  break;

  case 136:
#line 330 "Parser.y"
  {  (yyval.exprList) = new ast::ExprList(); ;}
  break;

  case 137:
#line 333 "Parser.y"
  {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr)); ;}
  break;

  case 138:
#line 334 "Parser.y"
  {  (yyval.exprList) = new ast::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr)); ;}
  break;

  case 139:
#line 337 "Parser.y"
  {  (yyval.constant) = new ast::Constant(true); ;}
  break;

  case 140:
#line 338 "Parser.y"
  {  (yyval.constant) = new ast::Constant(false); ;}
  break;

  case 141:
#line 339 "Parser.y"
  {  (yyval.constant) = new ast::Constant(0); ;}
  break;

  case 142:
#line 340 "Parser.y"
  {  (yyval.constant) = new ast::Constant((yyvsp[(1) - (1)].cVal)); ;}
  break;

  case 143:
#line 341 "Parser.y"
  {  (yyval.constant) = new ast::Constant((yyvsp[(1) - (1)].iVal)); ;}
  break;

  case 144:
#line 342 "Parser.y"
  {  (yyval.constant) = new ast::Constant((yyvsp[(1) - (1)].dVal)); ;}
  break;

  case 145:
#line 343 "Parser.y"
  {  (yyval.constant) = new ast::GlobalString(*(yyvsp[(1) - (1)].strVal)); ;}
  break;


  /* Line 1267 of yacc.c.  */
#line 2845 "parser.cpp"
  default: break;
  }
  YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK(yylen);
  yylen = 0;
  YY_STACK_PRINT(yyss, yyssp);

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
    yyerror(YY_("syntax error"));
#else
    {
      YYSIZE_T yysize = yysyntax_error(0, yystate, yychar);
      if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
      {
        YYSIZE_T yyalloc = 2 * yysize;
        if (!(yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
          yyalloc = YYSTACK_ALLOC_MAXIMUM;
        if (yymsg != yymsgbuf)
          YYSTACK_FREE(yymsg);
        yymsg = (char*)YYSTACK_ALLOC(yyalloc);
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
        (void)yysyntax_error(yymsg, yystate, yychar);
        yyerror(yymsg);
      }
      else
      {
        yyerror(YY_("syntax error"));
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
      yydestruct("Error: discarding",
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
  YYPOPSTACK(yylen);
  yylen = 0;
  YY_STACK_PRINT(yyss, yyssp);
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


    yydestruct("Error: popping",
      yystos[yystate], yyvsp);
    YYPOPSTACK(1);
    yystate = *yyssp;
    YY_STACK_PRINT(yyss, yyssp);
  }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

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
  yyerror(YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
    yydestruct("Cleanup: discarding lookahead",
      yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK(yylen);
  YY_STACK_PRINT(yyss, yyssp);
  while (yyssp != yyss)
  {
    yydestruct("Cleanup: popping",
      yystos[*yyssp], yyvsp);
    YYPOPSTACK(1);
  }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE(yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID(yyresult);
}


#line 345 "Parser.y"

