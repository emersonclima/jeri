%{
#include "stdio.h"
#include "ast.h"

extern int yylex(); 
int yyerror(void **tag, const char *p) { printf("erro: %s\n", p); }
%}

%pure-parser
%parse-param {void **tag}

%union {
	float fval;
	void* pval;
}

%token <pval> TRUE FALSE LPAREN RPAREN NUMBER
%left <pval> AND OR NOT
%type <pval> bool_exp
%%

bool_exp:	bool_exp AND bool_exp			{ $$ = (void*) createNode(AST_AND, (ASTNode*) $1, (ASTNode*) $3); *tag = $$; }
			| bool_exp OR bool_exp			{ $$ = (void*) createNode(AST_OR , (ASTNode*) $1, (ASTNode*) $3); *tag = $$; }
			| NOT bool_exp %prec NOT 		{ $$ = (void*) createNode(AST_NOT, (ASTNode*) $2, NULL         ); *tag = $$; }
			| LPAREN bool_exp RPAREN		{ $$ = $2;                                                        *tag = $$; }
			| NUMBER						{ $$ = (void*) createNumber($<fval>1);                            *tag = $$; }
			| TRUE							{ $$ = (void*) createBool(true);                                  *tag = $$; }
			| FALSE							{ $$ = (void*) createBool(false);                                 *tag = $$; }
			;
%%