#include "stdlib.h"
#include "string.h"
#include "ast.h"
#include "stdio.h"

ASTNode *createNumber(float value) {
    ASTNode *aux = (ASTNode *) malloc(sizeof(ASTNode));
    aux->type = AST_NUMBER;
    aux->value.fval = value;
    aux->left = NULL;
    aux->right = NULL;
    return aux;
}

ASTNode *createBool(bool value) {
    ASTNode *aux = (ASTNode *) malloc(sizeof(ASTNode));
    aux->type = AST_BOOLEAN;
    aux->value.bval = value;
    aux->left = NULL;
    aux->right = NULL;
    return aux;
}

ASTNode *createString(char *string) {
    ASTNode *aux = (ASTNode *) malloc(sizeof(ASTNode));
    aux->type = AST_STRING;
    strncpy(aux->value.sval, string, AST_STRING_MAX_VALUE);
    aux->left = NULL;
    aux->right = NULL;
    return aux;
}

ASTNode *createNode(ASTNodeType op, ASTNode *left, ASTNode *right) {
    ASTNode *aux = (ASTNode *) malloc(sizeof(ASTNode));
    aux->type = op;
    aux->left = left;
    aux->right = right;
    return aux;
}

static void print_node_value(ASTNode *n) {    
    if (n->type == AST_BOOLEAN) {        
        if (n->value.bval) {
            printf("true");            
        } else {
            printf("false");
        }
    } else if (n->type == AST_AND) {
        printf("and");
    } else if (n->type == AST_OR) {
        printf("or");
    } else if (n->type == AST_NOT) {
        printf("not");
    }
}

void print_dot_r(ASTNode* n) {
    printf("n%p [label=\"", n);
    print_node_value(n);   
    printf("\"]\n");
    if (n->left != NULL) print_dot_r(n->left);
    if (n->right != NULL) print_dot_r(n->right);

    if (n->left != NULL) printf("n%p -> n%p\n", n, n->left);
    if (n->right != NULL) printf("n%p -> n%p\n", n, n->right);    
}

void print_dot(ASTNode* n) {
    printf("digraph {\n");
    print_dot_r(n);
    printf("}\n");
}