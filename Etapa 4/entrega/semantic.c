// Autor: Leonardo de Souza Augusto - 278998

#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

int semanticErrors = 0;
int dataTypeMap[300] = {
    [AST_DECINT] = DATATYPE_INT,
    [AST_DEC_INTFUNC] = DATATYPE_INT,
    [AST_DECCHAR] = DATATYPE_CHAR,
    [AST_DEC_CHARFUNC] = DATATYPE_CHAR,
    [AST_DECFLOAT] = DATATYPE_FLOAT,
    [AST_DEC_FLOATFUNC] = DATATYPE_FLOAT
};

void checkAndSetDeclarations(AST *node){
    if (node == 0 || node == NULL)
        return;

    switch (node->type){
        case AST_FUNC_DECL:
            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_FUNCTION;
            else 
                printDeclarationError(node->son[0], "function");

            setDeclarationDataType(node);    
        break;
        case AST_DECL:  
            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_VARIABLE;
            else 
                printDeclarationError(node->son[0], "variable");

            setDeclarationDataType(node);    
        break;

    }

    for (int i=0; i < MAX_SONS; i++)
        checkAndSetDeclarations(node->son[i]);
}

void setDeclarationDataType(AST *node){
    if (node->son[0] == 0)
        return;
    if (node->son[0]->symbol == 0){
        fprintf(stderr, "Null Symbol when declaring data type \n");
        return;
    }

    node->son[0]->symbol->dataType = dataTypeMap[node->son[0]->type];
}

void checkUndeclared(){
    semanticErrors += hashCheckUndeclared();
}

void checkOperands(AST *node){
    if (node == 0 || node == NULL)
        return;

    switch (node->type){
        case AST_ADD:
            if (!isValidOperand(node->son[0])){
                printOperationError(node->son[0], "left", "Add");
                semanticErrors++;
            }
            if (!isValidOperand(node->son[1])){
                printOperationError(node->son[0], "right", "Add");
                semanticErrors++;
            }
        break;
    }

    for (int i=0; i < MAX_SONS; i++)
        checkOperands(node->son[i]);
}

void printDeclarationError(AST *node, char* errorMessage){
    if(node->symbol->text != NULL)
        fprintf(stderr, "Semantic Error: %s %s already declared\n", errorMessage, node->symbol->text);
    else
        fprintf(stderr, "Semantic Error: %s is already declared and text is null\n", errorMessage);
}

void printOperationError(AST *node, char* operand, char* operation){
    fprintf(stderr, "Semantic Error: invalid %s operand for %s\n", operand, operation);
}

int isValidDeclaration(AST *node){
    if (node->symbol != NULL){
        if (node->symbol->type != SYMBOL_IDENTIFIER){
            semanticErrors++;
            return FALSE;
        }
    }
    return TRUE;
}

int isValidOperand(AST *node){
    switch (node->type){
        case AST_SYMBOL:
            if (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR){
                return TRUE;
            }
            return FALSE;
        break;

        default: return FALSE;
    }
}

int getSemanticErrors(){
    return semanticErrors;
}