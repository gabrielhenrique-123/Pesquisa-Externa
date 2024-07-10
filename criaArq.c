#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ASI.h"


char* generateRandomString(int tam) {
    char *dado = (char*) malloc((tam + 1)* sizeof(char));
    char string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = strlen(string);
    for(int i = 0; i < tam; i++)
        dado[i] = string[rand()%stringLen];
    dado[tam] = '\0';
    return dado;
}

void arquivo(int qtdRegistros, int situacao){
    FILE *arquivo, *arquivoBin;
    Item item;
    memset(&item, 0, sizeof(item));
    int chaves[qtdRegistros];
    int contador = 0;
    long int dado1 = 0;
    char *dado2, *dado3;

    if((arquivo = fopen("arquivo.txt", "w")) == NULL) {
        puts("Arquivo nao pode ser aberto...");
        return;
    }
    if((arquivoBin = fopen("arquivo.bin", "wb+")) == NULL) {
        puts("Arquivo nao pode ser aberto...");
        return;
    }

    for(int i = 0; i < qtdRegistros; i++){
        chaves[i] = i + 1;
    }if (situacao == 3){
        for (int i = qtdRegistros - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = chaves[i];
            chaves[i] = chaves[j];
            chaves[j] = temp;
        }
    }
    
    while (contador < qtdRegistros){
        item.dado1 = 0;
        if(situacao == 2){
            item.chave = chaves[qtdRegistros - 1 - contador];
        }else{
            item.chave = chaves[contador];
        } 
        dado1 = rand();
        dado2 = generateRandomString(10);
        dado3 = generateRandomString(50);
        item.dado1 = dado1;
        strcpy(item.dado2, dado2);
        strcpy(item.dado3, dado3);
        fprintf(arquivo, "%d %ld %s %s\n", item.chave, item.dado1, item.dado2, item.dado3);
        fwrite(&item, sizeof(item), 1, arquivoBin);
        free(dado2);
        free(dado3);
        contador++;
    }
    fclose(arquivo);
    fclose(arquivoBin);
}

