#ifndef TACS_HEADER
#define TACS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

enum{
    TAC_SYMBOL = 500,
    TAC_ADD,
    TAC_SUB,
    TAC_DIV,
    TAC_MUL,
    TAC_LESS,
    TAC_GREATER,
    TAC_GE,
    TAC_LE,
    TAC_EQ,
    TAC_DIF,
    TAC_COPY,
    TAC_ARR_COPY,
    TAC_JMP_FALSE,
    TAC_JMP_TRUE,
    TAC_LABEL,
    TAC_BEGIN_FUNC,
    TAC_END_FUNC,
    TAC_RETURN,
    TAC_PRINT,
    TAC_READ,
    TAC_FUNC_CALL,
    TAC_FUNC_EXPR_LIST,
    TAC_GOTO
};

typedef struct tac_node{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC *tacCreate (int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC *tac);
void tacPrintRecursive(TAC *tac);
TAC *tacJoin(TAC *l1, TAC *l2);

// CODE GENERATION

TAC *generateCode(AST *node);
#endif