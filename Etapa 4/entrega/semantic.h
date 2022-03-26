#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

extern int semanticErrors;

void checkAndSetDeclarations(AST *node);
void checkUndeclared();

#endif