#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){
    FILE *arq;
    int num, count = 0;
    char c[10];

    arq = fopen("in2.txt", "r");
    if (arq == NULL){
        printf("erro\n");
        return 0;
    }

    while (fscanf(arq, "%s ", c) == 1){
        if(strchr(c, ';') != NULL){
            if (strlen(c) > 1 )
                count++;
            printf("%d ", count);
            count = 0;
        }
        else{
            count++;
        }
    }

    fclose(arq);
    return 0;
};