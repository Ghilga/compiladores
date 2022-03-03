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
        case AST_LCMD: fprintf(stderr, "AST_LCMD"); break;
        case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
        case AST_LABEL: fprintf(stderr, "AST_LABEL"); break;
        case AST_DEC: fprintf(stderr, "AST_DEC"); break;
        case AST_DECL: fprintf(stderr, "AST_DECL"); break;
        case AST_DECFUNC: fprintf(stderr, "AST_DECFUNC"); break;
        case AST_DECINT: fprintf(stderr, "AST_DECINT"); break;
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