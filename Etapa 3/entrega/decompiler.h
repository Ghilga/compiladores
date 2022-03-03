#include "ast.h"

void decompile(AST *tree, FILE * outputFile);
void decompileDecl(AST *node);
void decompileDecIntChar(AST *node);
void decompileDecFloat(AST *node);
void decompileArray(AST *node);
void decompileDecFunc(AST *node);
void decompileArgList(AST *node);
void decompileCmd(AST *node);
void decompileExpr(AST *node);
void decompileCompareAndArithmetic(AST *node, char* text);
void decompilePrintArgs(AST *node);