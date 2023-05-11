%{
#include <stdio.h>
#include <string>
#include "ast.hpp"
#include "parser.hpp"

extern "C" int yywrap() {return 1;}

char EscapeOfChar(char ch){
	if ('0'<=ch && ch<='9')
		return (char)(ch-'0');
	switch(ch){
	case 'a': return '\a';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	case 'v': return '\v';
	case '\\': return '\\';
	case '\'': return '\'';
	case '\"': return '\"';
	default:
		return ch;
	}
}
%}

%option never-interactive
%option yylineno
%option outfile="lexer.cpp"

%%
";"														{return SEMI;}
","														{return COMMA;}
"."														{return DOT;}
"..."													{return ELLIPSES;}
"("														{return LPAREN;}
")"														{return RPAREN;}
"{"														{return LBRACE;}
"}"														{return RBRACE;}
"["														{return LBRACKET;}
"]"														{return RBRACKET;}
"<<="													{return SHLEQ;}
"<<"													{return SHL;}
">>="													{return SHREQ;}
">>"													{return SHR;}
"=="													{return EQ_;}
">="													{return GEQ;}
">"														{return GT_;}
"<="													{return LEQ;}
"<"														{return LT_;}
"!="													{return NEQ;}
"!"														{return NOT;}
"="														{return ASSIGN;}
"&&"													{return AND;}
"&="													{return BANDEQ;}
"&"														{return BAND;}
"||"													{return OR;}
"|="													{return BOREQ;}
"|"														{return BOR;}
"->"													{return ARROW;}
"^="													{return BXOREQ;}
"^"														{return BXOR;}
"~"														{return BNOT;}
"++"													{return DADD;}
"+="													{return ADDEQ;}
"+"														{return ADD;}
"--"													{return DSUB;}
"-="													{return SUBEQ;}
"-"														{return SUB;}
"*="													{return MULEQ;}
"*"														{return MUL;}
"/="													{return DIVEQ;}
"/"														{return DIV;}
"%="													{return MODEQ;}
"%"														{return MOD;}
"?"														{return QUES;}
":"														{return COLON;}
"func"													{return FUNC;}
"struct"												{return STRUCT;}
"typedef"												{return TYPEDEF;}
"typecast"												{return TYPECAST;}
"const"													{return CONST;}
"ptr"													{return PTR;}
"dptr"                                                  {return DPTR;} 
"addr"                                                  {return ADDR;}
"array"													{return ARRAY;}
"if"													{return IF;}
"else"													{return ELSE;}
"for"													{return FOR;}
"switch"												{return SWITCH;}
"case"													{return CASE;}
"default"												{return DEFAULT;}
"continue"												{return CONTINUE;}
"return"												{return RETURN;}
"true"													{return TRUE; }
"false"													{return FALSE; }
"null"													{return NULL_; }
"break"													{return BREAK; }
"sizeof"												{return SIZEOF; }
"bool"													{return BOOL; }
"short"													{return SHORT; }
"int"													{return INT; }
"long"													{return LONG; }
"char"													{return CHAR; }
"float"													{return FLOAT; }
"double"												{return DOUBLE; }
"void"													{return VOID; }
[ \t\n]													{ ; }
"/*"[^*]*[*]+([^*/][^*]*[*]+)*"/"						{ ; }
"//".*													{ ; }
"\'"\\."\'"												{ 
															yylval.cVal = EscapeOfChar(yytext[2]);
															return CHARACTER; 
														}
"\'"[^\\']"\'"											{ 
															yylval.cVal = yytext[1];
															return CHARACTER; 
														}
"\'"													{return SQUOTE; }
"\""(\\.|[^"\\])*"\""									{
															yylval.strVal = new std::string("");
															for (int i = 1; i <= yyleng-2; i++)
																if (yytext[i] == '\\'){
																	i++;
																	yylval.strVal->push_back(EscapeOfChar(yytext[i]));
																}else{
																	yylval.strVal->push_back(yytext[i]);
																}
															return STRING;
														}
"\""													{return DQUOTE;}
[a-zA-Z_][a-zA-Z0-9_]*									{ 
															yylval.sVal = new std::string(yytext, yyleng);
															return IDENTIFIER; 
														} 
[0-9]+\.[0-9]+											{ 
															double dtmp;
															sscanf(yytext, "%lf", &dtmp);
															yylval.dVal = dtmp;
															return REAL; 
														}
[0-9]+													{
															int itmp;
															sscanf(yytext, "%d", &itmp);
															yylval.iVal = itmp; 
															return INTEGER; 
														}
%%