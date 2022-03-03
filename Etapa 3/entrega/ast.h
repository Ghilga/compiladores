// Abstract Syntax Tree

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

enum{
    AST_SYMBOL = 1,
    AST_ADD,
    AST_SUB,
    AST_LCMD,
    AST_ATTR,
    AST_LABEL,
    AST_DEC,
    AST_DECL,
    AST_DECFUNC,
    AST_DECINTCHAR,
    AST_PRINT,
    AST_WHILE,
    AST_IF,
    AST_GOTO,
    AST_RETURN,
    AST_ELSE,
    AST_ARR_ELEMENT
}



#include "hash.h"

typedef struct astnode
{
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
}AST;

AST *astCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3);
AST *astCreateSymbol(HASH_NODE *symbol);
void astPrint(AST *node, int level);
void printSpaces(int numSpaces);

#endif