//Autor: Leonardo de Souza Augusto - 278998

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"


extern int isRunning();
extern int yylex();
extern void initMe();
extern void hashPrint();
extern int yyparse();
extern char *yytext;
extern FILE *yyin;

FILE* initFile(char *filename) {
    FILE *inputFile;
    if ((inputFile = fopen(filename, "r")) == NULL) {
        perror("Error opening file");
        exit(2);
    }
    return inputFile;
}

int main(int argc, char **argv) {
    
    if (argc < 2) {
        printf("Call program with 'filename.txt'\n");
        exit(1);
    }

    initMe();
    FILE *inputFile = initFile(argv[1]);
    yyin = inputFile;
    int token;

    yyparse();

    hashPrint();
    printf("Success!\n");
    exit(0);
}
