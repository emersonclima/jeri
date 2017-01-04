%{
#include "parser.h"
#include "ast.h"
%}

%option noyywrap

ws      	[ \r\n\t]+
alpha		[A-Za-z]
digit		[0-9]
integer 	{digit}+
float		[0-9]+"."[0-9]*
number		({integer}|{float})
string  	\"[^\n\"]*\"
field		(serial|name|altloc|resname|chain|res|icode|x|y|z|occupancy|tempfactor|element|charge)

%%

{ws}   		/* skip blanks and tabs */
and 		{ return AND; }
or 			{ return OR; }
not 		{ return NOT; }
"(" 		{ return LPAREN; }
")"			{ return RPAREN; }
true		{ return TRUE; }
false		{ return FALSE; }
.        	{ printf("Unrecognized token!"); exit(1); }

%%