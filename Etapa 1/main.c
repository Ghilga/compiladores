//Autor: Leonardo de Souza Augusto - 278998

#include <stdio.h>
#include "tokens.h"
#include "hash.h"

extern int running;
extern int isRunning();
extern int yylex();
extern FILE* yyin;
FILE *inputFile;

void initMe(char* filename){
    running = 1;

    hashInit();

    if ( (inputFile = fopen(filename, "r")) == NULL ) {
        printf("Error opening file\n");
        exit(1);
    }

}

int main (int argc, char** argv) {
    if (argc < 2){
       printf("Call program with 'filename.txt'\n");
       exit(1);
    }

    initMe(argv[1]);
    yyin = inputFile;

    while(isRunning()){
        int token = yylex();
        if (token == 0){
            printf("Error %d\n", TOKEN_ERROR);
        }
        printf("Token retornado: %d, %c\n", token, token);
    }
    exit(0);
}

