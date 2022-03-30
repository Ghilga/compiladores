// Autor: Leonardo de Souza Augusto - 278998

#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

int semanticErrors = 0;
int dataTypeMap[300] = {
    [AST_DECINT] = DATATYPE_INT,
    [AST_DEC_INTFUNC] = DATATYPE_INT,
    [AST_ARGINT] = DATATYPE_INT,
    [AST_DECCHAR] = DATATYPE_CHAR,
    [AST_DEC_CHARFUNC] = DATATYPE_CHAR,
    [AST_ARGCHAR] = DATATYPE_CHAR,
    [AST_DECFLOAT] = DATATYPE_FLOAT,
    [AST_DEC_FLOATFUNC] = DATATYPE_FLOAT,
    [AST_ARGFLOAT] = DATATYPE_FLOAT
};

void checkAndSetDeclarations(AST *node){
    if (node == 0 || node == NULL)
        return;

    switch (node->type){
        case AST_FUNC_DECL:
            if (isValidDeclaration(node->son[0])){
                node->son[0]->symbol->type = SYMBOL_FUNCTION;
                node->son[0]->symbol->totalArguments = countFunctionArguments(node->son[0]->son[0]);
            } 
            else 
                printRedeclarationError(node->son[0], "function");

            setDeclarationDataType(node->son[0]);    
        break;
        case AST_DECL: 
            // Check if it is an array declaration
            if (isArrayDeclaration(node->son[0])){
                break;
            }

            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_VARIABLE;
            else { 
                printRedeclarationError(node->son[0], "variable");
                semanticErrors++;            
            }

            setDeclarationDataType(node->son[0]);    
        break;
        case AST_ARGLIST:
            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_ARGUMENT;
            else {
                printRedeclarationError(node->son[0], "argument");
                semanticErrors++;
            }
            setDeclarationDataType(node->son[0]);
        break;
        case AST_DECINT:
        case AST_DECCHAR:
        case AST_DECFLOAT:
            if (node->son[0] != 0){
                if (node->son[0]->type == AST_ARRAY){
                    // Check redeclaration
                    if (isValidDeclaration(node))
                        node->symbol->type = SYMBOL_VECTOR;
                    else 
                        printRedeclarationError(node, "array");
                    // Check size and initialization of array
                    if (isValidArrayDeclaration(node->son[0])){
                        setDeclarationDataType(node);
                    }
                    else{
                        fprintf(stderr,"Semantic Error: Wrong declaration of array '%s'\n", node->symbol->text);
                        semanticErrors++;
                    }
                }
            }
        break;
        case AST_LABEL:
            if (isValidDeclaration(node->son[0]))
                node->son[0]->symbol->type = SYMBOL_LABEL;
            else{
                printRedeclarationError(node->son[0], "label");
                semanticErrors++;
            }
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

    node->symbol->dataType = dataTypeMap[node->type];
}

void checkCommands(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        // TK_IDENTIFIER '=' expr
        // TK_IDENTIFIER '[' expr ']' '=' expr
        case AST_ATTR: 
        break;
        case AST_WHILE: // KW_WHILE expr cmd
        case AST_IF: // KW_IF expr KW_THEN cmd if_body
            if (isValidBoolean(node->son[0])){
                checkCommands(node->son[1]);
            }
            else {
                semanticErrors++;
                if (node->type == AST_WHILE)
                    printExpressionError("WHILE");
                else
                    printExpressionError("IF");
            }
        break;
        case AST_GOTO: // KW_GOTO TK_IDENTIFIER
            if (!(node->son[0]->symbol->type == SYMBOL_LABEL)){
                printExpressionError("GOTO");
                semanticErrors++;
            }

        break;
        case AST_RETURN: // KW_RETURN expr
        break;
    }

    for (int i=0; i < MAX_SONS; i++)
        checkCommands(node->son[i]);
}

void checkUndeclared(){
    semanticErrors += hashCheckUndeclared();
}

void checkNumericOperands(AST *node, char* operation){
    if (!isValidNumericOperand(node->son[0])){
        printOperationError("left", operation);
        semanticErrors++;
    }
    if (!isValidNumericOperand(node->son[1])){
        printOperationError("right", operation);
        semanticErrors++;
    }
}

void checkBooleanOperands(AST *node, char* operation){
    if (!isValidBooleanOperand(node->son[0])){
        printOperationError("left", operation);
        semanticErrors++;
    }
    if (!isValidBooleanOperand(node->son[1])){
        printOperationError("right", operation);
        semanticErrors++;
    }
}

void checkExpressions(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        case AST_ADD:
            checkNumericOperands(node, "ADD");
        break;
        case AST_SUB:
            checkNumericOperands(node, "SUBTRACT");
        break;
        case AST_MUL:
            checkNumericOperands(node, "MULTIPLICATE");
        break;
        case AST_DIV:
            checkNumericOperands(node, "DIVIDE");
        break;
        case AST_GREATER:
            checkBooleanOperands(node, "GREATER");
        break;
        case AST_LESS:
            checkBooleanOperands(node, "LESSER");
        break;
        case AST_DIF:
            checkBooleanOperands(node, "NOT EQUAL");
        break;
        case AST_EQ:
            checkBooleanOperands(node, "EQUALS");
        break;
        case AST_GE:
            checkBooleanOperands(node, "GE");
        break;
        case AST_LE:
            checkBooleanOperands(node, "LE");
        break;
            
    }

    for (int i=0; i < MAX_SONS; i++)
        checkExpressions(node->son[i]);
}

int isValidBoolean(AST *node){
    if (node == 0)
        return FALSE;
    switch (node->type){
        case AST_GREATER:
        case AST_LESS:
        case AST_DIF:
        case AST_EQ:
        case AST_GE:
        case AST_LE: return TRUE;
        break;
        default: return FALSE;
        break;
    }
}

int isValidArrayDeclaration(AST *node){
    if (node->symbol == 0)
        return FALSE;

    int arraySize = atoi(node->symbol->text);
    if (arraySize == 0)
        return FALSE;
    if (arraySize == 1 && node->son[0] != 0){
        if (node->son[0]->son[0] == 0)
            return TRUE;
        return FALSE;
    }

    AST *currentArrElement = node->son[0];
    for (int i=0; i < arraySize; i++){
        if(currentArrElement == 0)
            return TRUE;
        currentArrElement = currentArrElement->son[0];
    }

    if (currentArrElement == 0)
        return TRUE;

    return FALSE;
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

int isValidNumericOperand(AST *node){
    switch (node->type){
        case AST_SYMBOL:
            if (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR){
                return TRUE;
            }
            return FALSE;
        break;
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
            return TRUE;
        break;
        default: return FALSE;
    }
}

int isValidBooleanOperand(AST *node){
    switch (node->type){
        case AST_SYMBOL:
            if (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR){
                return TRUE;
            }
            return FALSE;
        break;
        case AST_LESS:
        case AST_GREATER:
        case AST_DIF:
        case AST_EQ:
        case AST_GE:
        case AST_LE:
            return TRUE;
        break;
        default: return FALSE;
    }
}

int isArrayDeclaration(AST *node){
    if (node->son[0] == 0)
        return FALSE;
    if (node->son[0]->type == AST_ARRAY)
        return TRUE;
    return FALSE;
}

int getSemanticErrors(){
    return semanticErrors;
}

int countFunctionArguments(AST *node){
    if (node == 0)
        return 0;
    
    if (node->type == AST_ARGLIST){
        return 1 + countFunctionArguments(node->son[1]);
    }

    return 0;
}

void printRedeclarationError(AST *node, char* errorMessage){
    if(node->symbol->text != NULL)
        fprintf(stderr, "Semantic Error: %s %s already declared\n", errorMessage, node->symbol->text);
    else
        fprintf(stderr, "Semantic Error: %s is already declared and text is null\n", errorMessage);
}

void printOperationError(char* operand, char* operation){
    fprintf(stderr, "Semantic Error: invalid %s operand for %s\n", operand, operation);
}

void printExpressionError(char* command){
    fprintf(stderr,"Semantic Error: Invalid %s expression\n", command);
}