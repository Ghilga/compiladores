//Autor: Leonardo de Souza Augusto - 278998

#include <stdio.h>

extern int running;

int main (){
    printf("Lex Started!\n");
    running = 1;
    yylex();
    running = 0;
}