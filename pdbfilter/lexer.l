%{
#include "stdlib.h"
#include "ast.h"
#include "parser.h"
%}

%option noyywrap
%option bison-bridge

ws      	[ \r\n\t]+
alpha		[A-Za-z]
digit		[0-9]
integer 	{digit}+
float		[0-9]+"."[0-9]*
number		({integer}|{float})
string  	\"[^\n\"]*\"

%%

"(" 		{ return LPAREN; }
")"			{ return RPAREN; }
and 		{ return AND; }
or 			{ return OR; }
not 		{ return NOT; }
true		{ return TRUE; }
false		{ return FALSE; }

{number}	{ yylval->fval = atof(yytext); return NUMBER; }
{ws}   		/* skip blanks and tabs */
.        	{ printf("Unrecognized token!"); exit(1); }

%%