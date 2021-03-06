// Autor: Leonardo de Souza Augusto - 278998

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
};

TAC *makeIfThen(TAC *code0, TAC *code1);
TAC *makeIfThenElse(TAC *code0, TAC *code1, TAC *code2);
TAC *makeBoolAndArithmetic(int nodeType, TAC *code0, TAC *code1);
TAC *makeCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1);
TAC *makeArrCopy(HASH_NODE *nodeSymbol, TAC *code0, TAC *code1, TAC *code2);
TAC *makeFunc(HASH_NODE *nodeSymbol, TAC *functionBody);
TAC *makeFuncCall(HASH_NODE *nodeSymbol, TAC *funcArgs);
TAC *makeExprList(TAC *code0, TAC *code1); 
TAC *makeCodeLabel(HASH_NODE *nodeSymbol, TAC *code0);
TAC *makeWhile(TAC *code0, TAC *code1); 
TAC *makeArrElement(HASH_NODE *nodeSymbol, TAC *code0); 

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
        case TAC_BEGIN_FUNC: fprintf(stderr,"TAC_BEGIN_FUNC"); break;
        case TAC_END_FUNC: fprintf(stderr,"TAC_END_FUNC"); break;
        case TAC_RETURN: fprintf(stderr,"TAC_RETURN"); break;
        case TAC_PRINT: fprintf(stderr,"TAC_PRINT"); break;
        case TAC_READ: fprintf(stderr,"TAC_READ"); break;
        case TAC_FUNC_CALL: fprintf(stderr,"TAC_FUNC_CALL"); break;
        case TAC_FUNC_EXPR_LIST: fprintf(stderr,"TAC_FUNC_EXPR_LIST"); break;
        case TAC_GOTO: fprintf(stderr,"TAC_GOTO"); break;
        case TAC_WHILE_BEGIN: fprintf(stderr,"TAC_WHILE_BEGIN"); break;
        case TAC_WHILE_END: fprintf(stderr,"TAC_WHILE_END"); break;
        case TAC_ARR_ELEMENT: fprintf(stderr,"TAC_ARR_ELEMENT"); break;
        
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
            // Array  
            if(node->son[1])
                result = makeArrCopy(node->symbol, code[0], code[1], code[2]); 
            // Variable
            else
                result = makeCopy(node->symbol, code[0], code[1]);
        break;
        case AST_IF: result = makeIfThen(code[0],code[1]); break;
        case AST_IF_ELSE: result = makeIfThenElse(code[0],code[1],code[2]); break;
        case AST_DEC_INTFUNC:
        case AST_DEC_CHARFUNC:
        case AST_DEC_FLOATFUNC: result = makeFunc(node->symbol, code[1]); break;
        case AST_RETURN: result = tacJoin(code[0], tacCreate(TAC_RETURN, code[0]?code[0]->res:0, 0, 0)); break;
        case AST_PRINTARGS:
        case AST_PRINT: result = tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0]?code[0]->res:0, 0, 0)), code[1]); break;
        case AST_READ: result = tacCreate(TAC_READ, makeTemp(), 0, 0); break;
        case AST_FUNC_CALL: result = makeFuncCall(node->symbol, code[0]); break;
        case AST_EXPR_LIST: result = makeExprList(code[0],code[1]); break;
        case AST_LABEL: result = makeCodeLabel(node->symbol,code[0]); break;
        case AST_GOTO: result = tacCreate(TAC_GOTO,node->symbol,0,0); break;
        case AST_WHILE: result = makeWhile(code[0],code[1]); break;
        case AST_ARR_ELEMENT: result = makeArrElement(node->symbol,code[0]); break;
        // Return the union of code for all subtrees
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
    TAC *whileEndLabelTac = 0;
    HASH_NODE *ifEndLabel = 0;
    HASH_NODE *whileEndLabel = 0;
    
    ifEndLabel = makeLabel();
    whileEndLabel = makeLabel();
    
    // Jumps
    ifFalseJumpTac = tacCreate(TAC_JMP_FALSE, ifEndLabel,code0->res,0);
    ifFalseJumpTac->prev = code0;

    ifTrueJumpTac = tacCreate(TAC_JMP_TRUE, whileEndLabel,code0->res,0);
    ifTrueJumpTac->prev = code1;

    // Labels
    ifEndLabelTac = tacCreate(TAC_LABEL,ifEndLabel,0,0);
    ifEndLabelTac->prev = ifTrueJumpTac;

    whileEndLabelTac = tacCreate(TAC_LABEL,whileEndLabel,0,0);
    whileEndLabelTac->prev = code2;
    
    return tacJoin(ifFalseJumpTac, tacJoin(ifEndLabelTac, whileEndLabelTac));
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

TAC *makeFunc(HASH_NODE *nodeSymbol, TAC *functionBody){
    return tacJoin(
        tacJoin(
            tacCreate(TAC_BEGIN_FUNC,nodeSymbol,0,0),
            functionBody
        ),
        tacCreate(TAC_END_FUNC,nodeSymbol,0,0)
    );
}

TAC *makeFuncCall(HASH_NODE *nodeSymbol, TAC*funcArgs){
    return tacJoin(
        tacJoin(
            funcArgs,
            tacCreate(
                TAC_FUNC_EXPR_LIST,
                funcArgs->res,
                0,
                0
            )
        ),
        tacCreate(TAC_FUNC_CALL, makeTemp(), nodeSymbol, 0)
    );
}

TAC *makeExprList(TAC *code0, TAC *code1){
    return tacJoin(
        tacJoin(
            code0, 
            tacCreate(
                TAC_FUNC_EXPR_LIST,
                code0->res,
                0,
                0
            )
        ),
        code1
    );
}

TAC *makeCodeLabel(HASH_NODE *nodeSymbol, TAC *code0){
    return tacJoin(
        tacCreate(TAC_LABEL,nodeSymbol,0,0),
        code0
    );
}

TAC *makeWhile(TAC *code0, TAC *code1){
    TAC *beginJumpTac = 0;
    TAC *endJumpTac = 0;
    TAC *beginLabelTac = 0;
    TAC *endLabelTac = 0;
    HASH_NODE *beginLabel = 0;
    HASH_NODE *endLabel = 0;

    // Labels
    beginLabel = makeLabel();
    endLabel = makeLabel();
    beginLabelTac = tacCreate(TAC_LABEL,beginLabel,0,0);
    endLabelTac = tacCreate(TAC_LABEL,endLabel,0,0);

    // Jumps
    endJumpTac = tacCreate(TAC_WHILE_BEGIN, endLabel,code0->res,0);
    endJumpTac->prev = code0;

    beginJumpTac = tacCreate(TAC_WHILE_END, beginLabel,0,0);
    beginJumpTac->prev = code1;

    endLabelTac->prev = beginJumpTac;
    
    return tacJoin(beginLabelTac, tacJoin(endJumpTac, endLabelTac));
}

TAC *makeArrElement(HASH_NODE *nodeSymbol, TAC *code0){
    return tacJoin(
        code0,
        tacCreate(TAC_ARR_ELEMENT,makeTemp(),nodeSymbol,code0->res)
    );
}