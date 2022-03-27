// Autor: Leonardo de Souza Augusto - 278998

#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

int semanticErrors = 0;
int dataTypeMap[100] = {
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

            setDeclarationDataType(node->son[0]);    
        break;
        case AST_DECL:  
            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_VARIABLE;
            else 
                printDeclarationError(node->son[0], "variable");

            setDeclarationDataType(node->son[0]);      
        break;

    }

    for (int i=0; i < MAX_SONS; i++)
        checkAndSetDeclarations(node->son[i]);
}

void setDeclarationDataType(AST *node){
    if (node == 0)
        return;
    if (node->symbol == 0){
        fprintf(stderr, "Null Symbol when declaring data type \n");
        return;
    }

    node->symbol->dataType = dataTypeMap[node->symbol->type];
}

void checkUndeclared(){
    semanticErrors += hashCheckUndeclared();
}

void printDeclarationError(AST *node, char* errorMessage){
    if(node->symbol->text != NULL)
        fprintf(stderr, "Semantic Error: %s %s already declared\n", errorMessage, node->symbol->text);
    else
        fprintf(stderr, "Semantic Error: %s is already declared and text is null\n", errorMessage);
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

int getSemanticErrors(){
    return semanticErrors;
}