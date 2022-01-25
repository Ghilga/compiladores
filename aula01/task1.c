#include <stdio.h>
#include <stdlib.h>

int main (){
    FILE *arq;
    int num, count = 0;
    char c[10];
    arq = fopen("in1.txt", "r");
    if (arq == NULL){
        printf("erro\n");
        return 0;
    }

    while (fscanf(arq, "%s ", c) == 1){
        printf("%s\n", c);
        count++;
    }

    printf("Total: %d\n", count);

    fclose(arq);
    return 0;
}