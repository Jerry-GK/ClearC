/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "Parser.y"
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
/* Line 1529 of yacc.c.  */
#line 259 "parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

