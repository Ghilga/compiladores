/*Autor: Leonardo de Souza Augusto - 278998 */

%%

teste           {printf("Teste bem sucedido!\n");}
0*              {printf("Sequencia de zeros\n");}
"is running?"     {printf(isRunning() ? "True" : "False");}

[ \n\t]

%%

#include "main.c"

int running = 0;


int getLineNumber () {
    // TODO 
    return 1;
}

int isRunning () {
    return running;
}

int yywrap() {
    return 1;
}