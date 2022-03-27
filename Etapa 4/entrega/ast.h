// Abstract Syntax Tree

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

#include "hash.h"

enum{
    AST_SYMBOL = 1,
    AST_ADD,
    AST_SUB,
    AST_CMD,
    AST_LCMD,
    AST_ATTR,
    AST_LABEL,
    AST_DECL,
    AST_FUNC_DECL,
    AST_DEC_INTFUNC,
    AST_DEC_CHARFUNC,
    AST_DEC_FLOATFUNC,
    AST_DECINT,
    AST_DECCHAR,
    AST_DECFLOAT,
    AST_PRINT,
    AST_WHILE,
    AST_IF,
    AST_GOTO,
    AST_RETURN,
    AST_ELSE,
    AST_ARR_ELEMENT,
    AST_ARGLIST,
    AST_ARGINT,
    AST_ARGCHAR,
    AST_ARGFLOAT,
    AST_FLOAT,
    AST_ARRAY,
    AST_ARR_VALUES,
    AST_FUNC_CALL,
    AST_MUL,
    AST_DIV,
    AST_LESS,
    AST_GREATER,
    AST_DIF,
    AST_EQ,
    AST_GE,
    AST_LE,
    AST_EXPR_LIST,
    AST_PRINTARGS
};


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