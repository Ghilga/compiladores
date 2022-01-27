//Autor: Leonardo de Souza Augusto - 278998

#include <stdio.h>


extern int isRunning();
//sdasd
extern int yylex();
extern char *yytext;
extern FILE *yyin;
FILE *inputFile;

void initMe(char *filename) {
    //hashInit();

    if ((inputFile = fopen(filename, "r")) == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Call program with 'filename.txt'\n");
        exit(1);
    }

    initMe(argv[1]);
    yyin = inputFile;
    int token = -1;

    while (isRunning()) {
        token = yylex();

        if (!isRunning()) {
            break;
        }
        printf("Texto: %s | Token: %d\n", yytext, token);
    }
    exit(0);
}
