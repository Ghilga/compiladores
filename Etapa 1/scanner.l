/*Autor: Leonardo de Souza Augusto - 278998 */
#include "main.c"
int isRunning = 0;

%%



teste           {printf("Teste bem sucedido!\n");}
0*              {printf("Sequencia de zeros\n");}

[ \n\t]

%%

int getLineNumber () {
    // TODO 
    return 1;
}

int isRunning () {
    return isRunning;
}

int yywrap() {
    return 1;
}