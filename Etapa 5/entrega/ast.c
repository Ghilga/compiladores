// Abstract Syntax Tree

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, 
               AST *s0, AST *s1, AST *s2, AST *s3){
    AST *newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    return newnode;
}

AST *astCreateSymbol(HASH_NODE *symbol){
    return astCreate(AST_SYMBOL, symbol, 0, 0, 0, 0);
}

void astPrint(AST *node, int level){
    if (node == 0)
        return;

    printSpaces(level);
    fprintf(stderr, "ast(");

    switch(node->type){
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_CMD: fprintf(stderr, "AST_CMD"); break;
        case AST_LCMD: fprintf(stderr, "AST_LCMD"); break;
        case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
        case AST_LABEL: fprintf(stderr, "AST_LABEL"); break;
        case AST_DECL: fprintf(stderr, "AST_DECL"); break;
        case AST_FUNC_DECL: fprintf(stderr, "AST_FUNC_DECL"); break;
        case AST_DEC_INTFUNC: fprintf(stderr, "AST_DEC_INTFUNC"); break;
        case AST_DEC_CHARFUNC: fprintf(stderr, "AST_DEC_CHARFUNC"); break;
        case AST_DEC_FLOATFUNC: fprintf(stderr, "AST_DEC_FLOATFUNC"); break;
        case AST_DECINT: fprintf(stderr, "AST_DECINT"); break;
        case AST_DECCHAR: fprintf(stderr, "AST_DECCHAR"); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
        case AST_IF: fprintf(stderr, "AST_IF"); break;
        case AST_IF_ELSE: fprintf(stderr, "AST_IF_ELSE"); break;
        case AST_GOTO: fprintf(stderr, "AST_GOTO"); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
        case AST_ARR_ELEMENT: fprintf(stderr, "AST_ARR_ELEMENT"); break;
        case AST_ARGLIST: fprintf(stderr, "AST_ARGLIST"); break;
        case AST_ARGINT: fprintf(stderr, "AST_ARGINT"); break;
        case AST_ARGCHAR: fprintf(stderr, "AST_ARGCHAR"); break;
        case AST_ARGFLOAT: fprintf(stderr, "AST_ARGFLOAT"); break;
        case AST_DECFLOAT: fprintf(stderr, "AST_DECFLOAT"); break;
        case AST_FLOAT: fprintf(stderr, "AST_FLOAT"); break;
        case AST_ARRAY: fprintf(stderr, "AST_ARRAY"); break;
        case AST_ARR_VALUES: fprintf(stderr, "AST_ARR_VALUES"); break;
        case AST_FUNC_CALL: fprintf(stderr, "AST_FUNC_CALL"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
        case AST_DIV: fprintf(stderr, "AST_DIV"); break;
        case AST_LESS: fprintf(stderr, "AST_LESS"); break;
        case AST_GREATER: fprintf(stderr, "AST_GREATER"); break;
        case AST_DIF: fprintf(stderr, "AST_DIF"); break;
        case AST_EQ: fprintf(stderr, "AST_EQ"); break;
        case AST_GE: fprintf(stderr, "AST_GE"); break;
        case AST_LE: fprintf(stderr, "AST_LE"); break;
        case AST_EXPR_LIST: fprintf(stderr, "AST_EXPR_LIST"); break;
        case AST_PRINTARGS: fprintf(stderr, "AST_PRINTARGS"); break;
        case AST_READ: fprintf(stderr, "AST_READ"); break;
        
        default: fprintf(stderr, "AST_UNKNOWN"); break;
    }
    if (node->symbol != 0)
        fprintf(stderr, ", %s", node->symbol->text);
    else
        fprintf(stderr, ", 0");
    
    fprintf(stderr, ")\n");
    
    for (int i=0; i < MAX_SONS; i++)
        astPrint(node->son[i], level+1);
    
}

void printSpaces(int numSpaces){
    for (int i=0; i < numSpaces; i++)
        fprintf(stderr, "  ");
}