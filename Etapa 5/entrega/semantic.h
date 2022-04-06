// Autor: Leonardo de Souza Augusto - 278998

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

#define TRUE 1
#define FALSE 0

void checkAndSetDeclarations(AST *node);
void setDeclarationDataType(AST *node);
void checkCommands(AST *node);
void checkUndeclared();
void checkOperands(AST *node);
void checkNumericOperands(AST *node, char *operation);
void checkBooleanOperands(AST *node, char* operation);
void checkExpressions(AST *node);
int isValidBoolean(AST *node);
int isValidArrayDeclaration(AST *node);
int isValidDeclaration(AST *node);
int isValidNumericOperand(AST *node);
int isValidBooleanOperand(AST *node);
int isArrayDeclaration(AST *node);
int countFunctionArguments(AST *node);
int getSemanticErrors();
void printRedeclarationError(AST *node, char* errorMessage);
void printOperationError(char* operand, char* operation); 
void printExpressionError(char* command);

#endif