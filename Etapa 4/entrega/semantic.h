// Autor: Leonardo de Souza Augusto - 278998

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

#define TRUE 1
#define FALSE 0

void checkAndSetDeclarations(AST *node);
void setDeclarationDataType(AST *node);
void checkUndeclared();
void checkOperands(AST *node);
void checkLeftAndRightOperands(AST *node, char *operation);
void printDeclarationError(AST *node, char* errorMessage);
void printOperationError(char* operand, char* operation); 
int isValidDeclaration(AST *node);
int isValidOperand(AST *node);
int getSemanticErrors();

#endif