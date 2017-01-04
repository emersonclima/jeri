/* Infix notation calculator--calc */

%{
#include "stdio.h"
#include "ast.h"

#define YYSTYPE ASTNode*
#define YYDEBUG 0

int yylex(); 
int yyerror(void **tag, const char *p) { printf("erro: %s\n", p); }
%}

%pure-parser
%parse-param {void **tag}

%token TRUE FALSE LPAREN RPAREN
%left AND OR NOT

//-- GRAMMAR RULES ---------------------------------------
%%

bool_exp:	| bool_exp AND bool_exp			{ $$ = createNode(AST_AND, $1, $3); *tag = (void*) $$; }
			| bool_exp OR bool_exp			{ $$ = createNode(AST_OR, $1, $3); *tag = (void*) $$; }
			| NOT bool_exp %prec NOT 		{ $$ = createNode(AST_NOT, $2, NULL); *tag = (void*) $$; }
			| LPAREN bool_exp RPAREN		{ $$ = $2; *tag = (void*) $$; }
			| TRUE							{ $$ = createBool(true); *tag = (void*) $$; }
			| FALSE							{ $$ = createBool(false); *tag = (void*) $$; }
			;

%%