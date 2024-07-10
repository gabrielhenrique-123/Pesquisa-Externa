#ifndef arvoreBinaria_h
#define arvoreBinaria_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "item.h"

typedef Item Registros;

typedef struct{
    Registros reg;
    int direita;
    int esquerda;
}RegArvore;

bool binaryTree(FILE*, int, Registros*, bool, int*, int*, double*, int*, int*, double*);
bool insereBinaria(FILE* , Registros, int, RegArvore, bool, int* );
bool searchTree(FILE* , int, Registros *, int*);
# endif