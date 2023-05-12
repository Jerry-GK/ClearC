%{
#include "ast.hpp" 
#include "../include/util.h"
#include <string>
#include <iostream>

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

void yyerror(char *s) {
    extern int yylineno;
    printError("[Syntax Error]: " + std::string(s) + " at line " + std::to_string(yylineno));
	std::exit(1); 
}

int yylex(void);

ast::Program *Root;
%}

%output="parser.cpp"

%union {
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

%token  SEMI COMMA DOT ELLIPSES SQUOTE DQUOTE QUES COLON
		LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
		SHLEQ SHL SHREQ SHR
		EQ_ GE GT_ LEQ LT_ NEQ NOT ASSIGN
		AND BANDEQ BAND OR BOREQ BOR
		ARROW BXOREQ BXOR BNOT
		DADD ADDEQ ADD DSUB SUBEQ SUB
		MULEQ MUL DIVEQ DIV MODEQ MOD
		FUNC STRUCT TYPEDEF TYPECAST THIS CONST PTR ARRAY DPTR ADDR
		IF ELSE FOR SWITCH CASE DEFAULT 
		BREAK CONTINUE RETURN SIZEOF TRUE FALSE NULL_
		BOOL SHORT INT LONG CHAR FLOAT DOUBLE VOID
		
%token<iVal> INTEGER
%token<sVal> IDENTIFIER 
%token<dVal> REAL
%token<cVal> CHARACTER
%token<strVal> STRING
%type<program>							Program	
%type<decl>								Decl	
%type<decls>							Decls	
%type<funcDecl>							FuncDecl
%type<funcBody>							FuncBody	
%type<varDecl>							VarDecl	
%type<typeDecl>							TypeDecl	
%type<varType>							VarType _VarType
%type<builtInType>						BuiltInType
%type<fieldDecls>						FieldDecls
%type<fieldDecl>						FieldDecl
%type<memList>							MemList _MemList	
%type<stmt>								Stmt
%type<ifStmt>							IfStmt
%type<forStmt>							ForStmt
%type<switchStmt>						SwitchStmt
%type<caseList>							CaseList
%type<caseStmt>							CaseStmt
%type<breakStmt>						BreakStmt
%type<continueStmt>						ContinueStmt
%type<returnStmt>						ReturnStmt
%type<stmts>							Stmts
%type<block>							Block
%type<arg>								Arg
%type<argList>							ArgList _ArgList
%type<varInit>							VarInit	
%type<varList>							VarList _VarList
%type<expr>								Expr	
%type<constant>							Constant
%type<exprList>							ExprList _ExprList

%nonassoc IF
%nonassoc ELSE

%left	COMMA //15
%left	FUNC_CALL_ARG_LIST
%right	ASSIGN ADDEQ SUBEQ MULEQ DIVEQ MODEQ SHLEQ SHREQ BANDEQ BOREQ BXOREQ //14
%right	QUES COLON //13
%left	OR//12
%left	AND//11
%left	BOR//10
%left	BXOR//9
%left	BAND//8
%left	EQ_ NEQ//7
%left	GEQ GT_ LEQ LT_//6
%left	SHL SHR//5
%left	ADD SUB//4
%left	MUL DIV MOD//3
%right	DADD DSUB NOT BNOT SIZEOF//2
%left	DOT ARROW//1

%start Program
%%
Program:	Decls													{  $$ = new ast::Program($1); Root = $$;   }
			;
			
Decls:		Decls Decl												{  $$ = $1; $$->push_back($2);   }
			|														{  $$ = new ast::Decls();   }
			;

Decl:		FuncDecl												{  $$ = $1;   }
			| VarDecl												{  $$ = $1;   }
			| TypeDecl												{  $$ = $1;   }
			;

FuncDecl:	FUNC IDENTIFIER LPAREN ArgList RPAREN ARROW VarType SEMI 	{  $$ = new ast::FuncDecl($7,*$2,$4);   }
			| FUNC IDENTIFIER LPAREN ArgList RPAREN ARROW VarType FuncBody {  $$ = new ast::FuncDecl($7,*$2,$4,"",$8);   }
			| FUNC IDENTIFIER COLON IDENTIFIER LPAREN ArgList RPAREN ARROW VarType SEMI {  $$ = new ast::FuncDecl($9,*$4,$6,*$2);   }
			| FUNC IDENTIFIER COLON IDENTIFIER LPAREN ArgList RPAREN ARROW VarType FuncBody {  $$ = new ast::FuncDecl($9,*$4,$6,*$2,$10);   }
			;

FuncBody:	LBRACE Stmts RBRACE										{  $$ = new ast::FuncBody($2);   }
			;

VarDecl:	VarType VarList	SEMI   									{  $$ = new ast::VarDecl($1,$2);   }
			;

VarList:	_VarList COMMA VarInit									{  $$ = $1; $$->push_back($3);   }
			| VarInit												{  $$ = new ast::VarList(); $$->push_back($1);   }											
			|														{  $$ = new ast::VarList();   }
			;

_VarList:	_VarList COMMA VarInit									{  $$ = $1; $$->push_back($3);   }
			| VarInit												{  $$ = new ast::VarList(); $$->push_back($1);   }	
			;

VarInit:	IDENTIFIER												{  $$ = new ast::VarInit(*$1);   }
			| IDENTIFIER ASSIGN Expr								{  $$ = new ast::VarInit(*$1,$3);   }
			;

TypeDecl:	TYPEDEF IDENTIFIER VarType SEMI						{  $$ = new ast::TypeDecl(*$2,$3);   }
			;

VarType:	_VarType												{  $$ = $1;   }
			| CONST _VarType										{  $$ = $2; $$->SetConst();   }
			;

_VarType:	BuiltInType												{  $$ = $1;   }
			| STRUCT LBRACE FieldDecls RBRACE						{  $$ = new ast::StructType($3);   }
			| PTR LT_ VarType GT_									{  $$ = new ast::PointerType($3);   }
			| ARRAY LT_ VarType COMMA INTEGER GT_					{  $$ = new ast::ArrayType($3,$5);   }
			| IDENTIFIER											{  $$ = new ast::DefinedType(*$1);   }
			;
			
BuiltInType: BOOL													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Bool);   }
			| SHORT													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Short);   }
			| INT													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Int);   }
			| LONG													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Long);   }
			| CHAR													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Char);   }
			| FLOAT													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Float);   }
			| DOUBLE												{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Double);   }
			| VOID													{  $$ = new ast::BuiltInType(ast::BuiltInType::TypeID::_Void);   }
			;

FieldDecls:	FieldDecls FieldDecl            						{  $$ = $1; if ($2 != NULL) $$->push_back($2);   }
			|														{  $$ = new ast::FieldDecls();   }
			;

FieldDecl:	VarType MemList	SEMI    								{  $$ = new ast::FieldDecl($1,$2);   }
			| SEMI													{  $$ = NULL;   }
			;

MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back(*$3);   }
			| IDENTIFIER											{  $$ = new ast::MemList(); $$->push_back(*$1);   }	
			|														{  $$ = new ast::MemList();   }
			;

_MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back(*$3);   }
			| IDENTIFIER											{  $$ = new ast::MemList(); $$->push_back(*$1);   }
			;

ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }
			| _ArgList COMMA ELLIPSES								{  $$ = $1; $$->SetVarArg();   }
			| Arg													{  $$ = new ast::ArgList(); $$->push_back($1);   }
			| ELLIPSES												{  $$ = new ast::ArgList(); $$->SetVarArg();   }
			|														{  $$ = new ast::ArgList();   }
			;

_ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }	 
			| Arg													{  $$ = new ast::ArgList(); $$->push_back($1);   }
			;

Arg:		VarType IDENTIFIER										{  $$ = new ast::Arg($1,*$2);   }
			| VarType												{  $$ = new ast::Arg($1);   }
			;

Block:		LBRACE Stmts RBRACE										{  $$ = new ast::Block($2);   }
			;

Stmts:		Stmts Stmt      										{  $$ = $1; if ($2 != NULL) $$->push_back($2);   }	
			|														{  $$ = new ast::Stmts();   }
			;

Stmt:		Expr SEMI												{  $$ = $1;   }
			| IfStmt												{  $$ = $1;   }
			| ForStmt												{  $$ = $1;   }
			| SwitchStmt											{  $$ = $1;   } 
			| BreakStmt												{  $$ = $1;   }
			| ContinueStmt											{  $$ = $1;   }
			| ReturnStmt											{  $$ = $1;   }
			| Block													{  $$ = $1;   }
			| VarDecl												{  $$ = $1;   }
			| TypeDecl												{  $$ = $1;   }
			| SEMI													{  $$ = NULL;   }
			;

IfStmt:		IF Expr Block ELSE Block					{  $$ = new ast::IfStmt($2,$3,$5);   }
			| IF Expr Block         					{  $$ = new ast::IfStmt($2,$3);   }
			;

ForStmt:	FOR Expr SEMI Expr SEMI Expr Block			{  $$ = new ast::ForStmt($2,$4,$6,$7);   }
            | FOR VarDecl Expr SEMI Expr Block			{  $$ = new ast::ForStmt($2,$3,$5,$6);   }
			| FOR Expr Block							{  $$ = new ast::ForStmt(NULL,$2,NULL,$3);   }
			| FOR Block									{  $$ = new ast::ForStmt(NULL,NULL,NULL,$2);   }
			;

SwitchStmt:	SWITCH Expr LBRACE CaseList RBRACE		{  $$ = new ast::SwitchStmt($2,$4);   }
			;

CaseList:	CaseList CaseStmt										{  $$ = $1; $$->push_back($2);   }	
			|														{  $$ = new ast::CaseList();   }
			;

CaseStmt:	CASE Expr COLON Stmts									{  $$ = new ast::CaseStmt($2,$4);   }
			| DEFAULT COLON Stmts									{  $$ = new ast::CaseStmt(NULL,$3);   }
			;


ContinueStmt:CONTINUE SEMI											{  $$ = new ast::ContinueStmt();   }
			;

BreakStmt:	BREAK SEMI												{  $$ = new ast::BreakStmt();   }
			;

ReturnStmt: RETURN SEMI 											{  $$ = new ast::ReturnStmt();   }
			| RETURN Expr SEMI										{  $$ = new ast::ReturnStmt($2);   }
			;

Expr:		Expr LBRACKET Expr RBRACKET %prec ARROW					{  $$ = new ast::Subscript($1,$3);   }
			| SIZEOF LPAREN IDENTIFIER RPAREN						{  $$ = new ast::SizeOf(*$3);   }
			| SIZEOF LPAREN Expr RPAREN								{  $$ = new ast::SizeOf($3);   }
			| SIZEOF LPAREN VarType RPAREN							{  $$ = new ast::SizeOf($3);   }
			| IDENTIFIER LPAREN ExprList RPAREN						{  $$ = new ast::FunctionCall(*$1,$3);   }
			| Expr DOT IDENTIFIER LPAREN ExprList RPAREN			{  $$ = new ast::FunctionCall(*$3,$5,$1,NULL);   }
			| Expr ARROW IDENTIFIER LPAREN ExprList RPAREN			{  $$ = new ast::FunctionCall(*$3,$5,NULL,$1);   }
			| Expr DOT IDENTIFIER									{  $$ = new ast::StructReference($1,*$3);   }
			| Expr ARROW IDENTIFIER									{  $$ = new ast::StructDereference($1,*$3);   }
			| ADD Expr	%prec NOT									{  $$ = new ast::UnaryPlus($2);   }
			| SUB Expr	%prec NOT									{  $$ = new ast::UnaryMinus($2);   }
			| TYPECAST LPAREN  Expr COMMA VarType RPAREN %prec NOT  {  $$ = new ast::TypeCast($5,$3);   }
			| DADD Expr	%prec NOT									{  $$ = new ast::PrefixInc($2);   }
			| Expr DADD %prec ARROW									{  $$ = new ast::PostfixInc($1);   }
			| DSUB Expr %prec NOT									{  $$ = new ast::PrefixDec($2);   }
			| Expr DSUB	%prec ARROW									{  $$ = new ast::PostfixDec($1);   }
			| DPTR LPAREN Expr RPAREN	%prec NOT					{  $$ = new ast::Indirection($3);   }
			| ADDR LPAREN Expr RPAREN	%prec NOT					{  $$ = new ast::AddressOf($3);   }
			| NOT Expr												{  $$ = new ast::LogicNot($2);   }
			| BNOT Expr												{  $$ = new ast::BitwiseNot($2);   }
			| Expr DIV Expr											{  $$ = new ast::Division($1,$3);   }
			| Expr MUL Expr											{  $$ = new ast::Multiplication($1,$3);   } 
			| Expr MOD Expr 										{  $$ = new ast::Modulo($1,$3);   }
			| Expr ADD Expr											{  $$ = new ast::Addition($1,$3);   } 
			| Expr SUB Expr											{  $$ = new ast::Subtraction($1,$3);   } 
			| Expr SHL Expr											{  $$ = new ast::LeftShift($1,$3);   } 
			| Expr SHR Expr											{  $$ = new ast::RightShift($1,$3);   } 
			| Expr GT_ Expr											{  $$ = new ast::LogicGT($1,$3);   } 
			| Expr GEQ Expr											{  $$ = new ast::LogicGE($1,$3);   } 
			| Expr LT_ Expr											{  $$ = new ast::LogicLT($1,$3);   } 
			| Expr LEQ Expr											{  $$ = new ast::LogicLE($1,$3);   } 
			| Expr EQ_ Expr											{  $$ = new ast::LogicEQ($1,$3);   } 
			| Expr NEQ Expr											{  $$ = new ast::LogicNEQ($1,$3);   } 
			| Expr BAND Expr										{  $$ = new ast::BitwiseAND($1,$3);   }
			| Expr BXOR Expr										{  $$ = new ast::BitwiseXOR($1,$3);   }
			| Expr BOR Expr											{  $$ = new ast::BitwiseOR($1,$3);   } 
			| Expr AND Expr											{  $$ = new ast::LogicAND($1,$3);   } 
			| Expr OR Expr											{  $$ = new ast::LogicOR($1,$3);   } 
			| Expr QUES Expr COLON Expr								{  $$ = new ast::TernaryCondition($1,$3,$5);   }
			| Expr ASSIGN Expr 										{  $$ = new ast::DirectAssign($1,$3);   } 
			| Expr DIVEQ Expr 										{  $$ = new ast::DivAssign($1,$3);   } 
			| Expr MULEQ Expr										{  $$ = new ast::MulAssign($1,$3);   }  
			| Expr MODEQ Expr										{  $$ = new ast::ModAssign($1,$3);   } 
			| Expr ADDEQ Expr										{  $$ = new ast::AddAssign($1,$3);   } 
			| Expr SUBEQ Expr										{  $$ = new ast::SubAssign($1,$3);   } 
			| Expr SHLEQ Expr										{  $$ = new ast::SHLAssign($1,$3);   } 
			| Expr SHREQ Expr										{  $$ = new ast::SHRAssign($1,$3);   } 
			| Expr BANDEQ Expr										{  $$ = new ast::BitwiseANDAssign($1,$3);   } 
			| Expr BXOREQ Expr										{  $$ = new ast::BitwiseXORAssign($1,$3);   } 
			| Expr BOREQ Expr										{  $$ = new ast::BitwiseORAssign($1,$3);   }
			| LPAREN Expr RPAREN									{  $$ = $2;   }
			| IDENTIFIER											{  $$ = new ast::Variable(*$1);   } 
			| Constant												{  $$ = $1;   }												
			| Expr COMMA Expr										{  $$ = new ast::CommaExpr($1, $3);   }
			| THIS													{  $$ = new ast::This();   }
			;

ExprList:	_ExprList COMMA Expr									{  $$ = $1; $$->push_back($3);   }
			| Expr %prec FUNC_CALL_ARG_LIST							{  $$ = new ast::ExprList(); $$->push_back($1);   }
			|														{  $$ = new ast::ExprList();   }
			;

_ExprList:	_ExprList COMMA Expr 									{  $$ = $1; $$->push_back($3);   }
			| Expr %prec FUNC_CALL_ARG_LIST							{  $$ = new ast::ExprList(); $$->push_back($1);   }
			;

Constant:	TRUE													{  $$ =  new ast::Constant(true);   }
			| FALSE													{  $$ =  new ast::Constant(false);   }
			| NULL_													{  $$ =  new ast::Constant(0);   }
			| CHARACTER												{  $$ =  new ast::Constant($1);   }
			| INTEGER 												{  $$ =  new ast::Constant($1);   }
			| REAL													{  $$ =  new ast::Constant($1);   }
			| STRING												{  $$ =  new ast::GlobalString(*$1);   }
			;
%%