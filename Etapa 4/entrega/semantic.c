#include "semantic.h"
#include <stdio.h>

int semanticErrors = 0;

void checkAndSetDeclarations(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        case AST_DEC_INTFUNC || AST_DEC_CHARFUNC || AST_DEC_FLOATFUNC:
            if (node->symbol != NULL){
                if (node->symbol->type != SYMBOL_IDENTIFIER){
                    if(node->symbol->text != NULL)
                        fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    else
                        fprintf(stderr, "Semantic ERROR: variable is already declared and text is null\n");
                }
            } else {
                fprintf(stderr, "Node symbol is NULL\n");
            }
            break;
            
        case AST_DECINT || AST_DECCHAR || AST_DECFLOAT:
            break;
    }

    for (int i=0; i < MAX_SONS; i++)
        checkAndSetDeclarations(node->son[i]);
}

void checkUndeclared(){

}
