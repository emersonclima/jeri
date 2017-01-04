#include <stdbool.h>

#define AST_STRING_MAX_VALUE 80

typedef union {
    bool bval;
    float fval;
    char sval[AST_STRING_MAX_VALUE + 1];
} ASTValue;

typedef enum {    
    AST_NUMBER,
    AST_BOOLEAN,
    AST_STRING,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_EQUALS,
    AST_NOT_EQUALS,
    AST_LESS_THAN,
    AST_LESS_EQUALS_THAN,
    AST_MORE_THAN,
    AST_MORE_EQUALS_THAN,
} ASTNodeType;

typedef struct ast_node {
    ASTValue value;
    ASTNodeType type;
    struct ast_node *left;
    struct ast_node *right;
} ASTNode;

ASTNode *createBool(bool value);
ASTNode *createNumber(float value);
ASTNode *createString(char *value);
ASTNode *createNode(ASTNodeType op, ASTNode *left, ASTNode *right);
void print_dot(ASTNode* n);
