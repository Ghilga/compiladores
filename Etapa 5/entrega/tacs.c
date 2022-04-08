#include "tacs.h"

int astToTacMap[50] = {
    [AST_ADD] = TAC_ADD,
    [AST_SUB] = TAC_SUB,
    [AST_DIV] = TAC_DIV,
    [AST_MUL] = TAC_MUL,
    [AST_LESS] = TAC_LESS,
    [AST_GREATER] = TAC_GREATER,
    [AST_GE] = TAC_GE,
    [AST_LE] = TAC_LE,
    [AST_EQ] = TAC_EQ,
    [AST_DIF] = TAC_DIF,
    [AST_ATTR] = TAC_COPY,
};

TAC *makeIfThen(TAC *code0, TAC *code1);
TAC *makeIfThenElse(TAC *code0, TAC *code1, TAC *code2);
TAC *makeBoolAndArithmetic(int nodeType, TAC *code0, TAC *code1);
TAC *makeCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1);
TAC *makeArrCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1, TAC *code2);

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
    if (!tac || tac->type == TAC_SYMBOL) return;
    fprintf(stderr,"TAC(");
    switch (tac->type){
        case TAC_COPY: fprintf(stderr,"TAC_COPY"); break;
        case TAC_ADD: fprintf(stderr,"TAC_ADD"); break;
        case TAC_SUB: fprintf(stderr,"TAC_SUB"); break;
        case TAC_DIV: fprintf(stderr,"TAC_DIV"); break;
        case TAC_MUL: fprintf(stderr,"TAC_MUL"); break;
        case TAC_LESS: fprintf(stderr,"TAC_LESS"); break;
        case TAC_GREATER: fprintf(stderr,"TAC_GREATER"); break;
        case TAC_GE: fprintf(stderr,"TAC_GE"); break;
        case TAC_LE: fprintf(stderr,"TAC_LE"); break;
        case TAC_EQ: fprintf(stderr,"TAC_EQ"); break;
        case TAC_DIF: fprintf(stderr,"TAC_DIF"); break;
        case TAC_JMP_FALSE: fprintf(stderr,"TAC_JMP_FALSE"); break;
        case TAC_JMP_TRUE: fprintf(stderr,"TAC_JMP_TRUE"); break;
        case TAC_ARR_COPY: fprintf(stderr,"TAC_ARR_COPY"); break;
        case TAC_LABEL: fprintf(stderr,"TAC_LABEL"); break;
        default: fprintf(stderr,"TAC_UNKNOWN"); break;
    }
    fprintf(stderr,", %s", (tac->res)?tac->res->text:"0");
    fprintf(stderr,", %s", (tac->op1)?tac->op1->text:"0");
    fprintf(stderr,", %s", (tac->op2)?tac->op2->text:"0");
    fprintf(stderr,");\n");
}

void tacPrintRecursive(TAC *tac){
    if (!tac) return;
    else{
        tacPrintRecursive(tac->prev);
        tacPrint(tac);
    }
}

TAC *tacJoin(TAC *l1, TAC *l2){
    TAC *point;
    if (!l1) return l2;
    if (!l2) return l1;
    for (point = l2; point->prev != 0; point = point->prev);
    point->prev = l1;
    return l2;
}

// CODE GENERATION

TAC *generateCode(AST *node){
    TAC *result = 0;
    TAC *code[MAX_SONS];
    
    if (!node) return 0;

// PROCESS CHILDREN
    for (int i=0; i<MAX_SONS; i++){
        code[i] = generateCode(node->son[i]);
    }
// PROCESS THIS CODE
    switch (node->type){
        case AST_SYMBOL: result = tacCreate(TAC_SYMBOL,node->symbol,0,0); break;
        case AST_ADD: 
        case AST_SUB:
        case AST_DIV:
        case AST_MUL:
        case AST_LESS:
        case AST_GREATER: 
        case AST_GE:
        case AST_LE:
        case AST_EQ:
        case AST_DIF: result = makeBoolAndArithmetic(node->type, code[0], code[1]); break;
        case AST_ATTR: 
            if(node->son[1])
                result = makeArrCopy(node->symbol, code[0], code[1], code[2]); 
            else
                result = makeCopy(node->symbol, code[0], code[1]);
        break;
        case AST_IF: result = makeIfThen(code[0],code[1]); break;
        case AST_IF_ELSE: result = makeIfThenElse(code[0],code[1],code[2]); break;

        //return the union of code for all subtrees
        default: result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2],code[3]))); break;    
        }

    return result;
}
TAC *makeIfThen(TAC *code0, TAC *code1){
    TAC *ifFalseJumpTac = 0;
    TAC *ifEndLabelTac = 0;
    HASH_NODE *ifEndLabel = 0;
    ifEndLabel = makeLabel();
    
    // Jumps
    ifFalseJumpTac = tacCreate(TAC_JMP_FALSE, ifEndLabel,code0->res,0);
    ifFalseJumpTac->prev = code0;

    // Labels
    ifEndLabelTac = tacCreate(TAC_LABEL,ifEndLabel,0,0);
    ifEndLabelTac->prev = code1;
    
    return tacJoin(ifFalseJumpTac, ifEndLabelTac);
}

TAC *makeIfThenElse(TAC *code0, TAC *code1, TAC *code2){
    TAC *ifFalseJumpTac = 0;
    TAC *ifTrueJumpTac = 0;
    TAC *ifEndLabelTac = 0;
    TAC *elseEndLabelTac = 0;
    HASH_NODE *ifEndLabel = 0;
    HASH_NODE *elseEndLabel = 0;
    
    ifEndLabel = makeLabel();
    elseEndLabel = makeLabel();
    
    // Jumps
    ifFalseJumpTac = tacCreate(TAC_JMP_FALSE, ifEndLabel,code0->res,0);
    ifFalseJumpTac->prev = code0;

    ifTrueJumpTac = tacCreate(TAC_JMP_TRUE, elseEndLabel,code0->res,0);
    ifTrueJumpTac->prev = code1;

    // Labels
    ifEndLabelTac = tacCreate(TAC_LABEL,ifEndLabel,0,0);
    ifEndLabelTac->prev = ifTrueJumpTac;

    elseEndLabelTac = tacCreate(TAC_LABEL,elseEndLabel,0,0);
    elseEndLabelTac->prev = code2;
    
    return tacJoin(ifFalseJumpTac, tacJoin(ifEndLabelTac, elseEndLabelTac));
}

TAC *makeBoolAndArithmetic(int nodeType, TAC *code0, TAC *code1){
    return tacJoin(tacJoin(code0,code1)
                ,tacCreate(astToTacMap[nodeType]
                    ,makeTemp()
                    ,code0?code0->res:0
                    ,code1?code1->res:0)
                    );
}

TAC *makeCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1){
    return tacJoin(code0
                ,tacCreate(TAC_COPY
                    ,nodeSymbol?nodeSymbol:makeTemp()
                    ,code0?code0->res:0
                    ,code1?code1->res:0)
                    ); 
}

TAC *makeArrCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1, TAC *code2){
    return tacJoin(code0
                ,tacCreate(TAC_ARR_COPY
                    ,nodeSymbol?nodeSymbol:makeTemp()
                    ,code0?code0->res:0
                    ,code1?code1->res:0)
                    ); 
}

