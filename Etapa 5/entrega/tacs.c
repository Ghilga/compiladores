#include "tacs.h"

TAC *tacCreate (int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2){
    TAC *newTac = 0;
    newTac = (TAC*) calloc(1,sizeof(TAC));
    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = 0;
    newTac->next = 0;
    return newTac;
}
void tacPrint(TAC *tac){
    if (!tac) return;
    fprintf(stderr,"TAC(");
    switch (tac->type){
        case TAC_SYMBOL: fprintf(stderr,"TAC_SYMBOL"); break;
        case TAC_ADD: fprintf(stderr,"TAC_ADD"); break;
        case TAC_SUB: fprintf(stderr,"TAC_SUB"); break;
        default: fprintf(stderr,"TAC_UNKNOWN"); break;
    }
    fprintf(stderr,",%s", (tac->res)?tac->res->text:"");
    fprintf(stderr,",%s", (tac->op1)?tac->op1->text:"");
    fprintf(stderr,",%s", (tac->op2)?tac->op2->text:"");
    fprintf(stderr,");\n");
}
void tacPrintBackwards(TAC *tac){

}

// CODE GENERATION

TAC *generate(AST *node){

}