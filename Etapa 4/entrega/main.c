// Autor: Leonardo de Souza Augusto - 278998

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "decompiler.h"
#include "semantic.h"


extern int isRunning();
extern int yylex();
extern void initMe();
extern void hashPrint();
extern int yyparse();
extern char *yytext;
extern FILE *yyin;
extern AST *fullAst;

FILE* initFile(char *filename, char *modes) {
    FILE *inputFile;
    if ((inputFile = fopen(filename, modes)) == NULL) {
        perror("Error opening file");
        exit(2);
    }
    return inputFile;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Call program with 'input.txt output.txt'\n");
        exit(1);
    }

    initMe();
    FILE *inputFile = initFile(argv[1], "r");
    FILE *outputFile = initFile(argv[2], "w");
    yyin = inputFile;

    yyparse();

    astPrint(fullAst,0);
    checkAndSetDeclarations(fullAst);
    checkUndeclared();
    hashPrint();
    int semanticErrors = getSemanticErrors();
    if (semanticErrors > 0){
        printf("Total semantic errors: %d\n", semanticErrors);
        exit(4);
    }
    decompile(fullAst, outputFile);
    printf("Success!\n");
    exit(0);
}
