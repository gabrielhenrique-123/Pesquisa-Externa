# ifndef ASI_h
# define ASI_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "item.h"
#define ITENSPORPAGINA 4

int acessoSequencialIndexado(Item *, FILE*, Analise*);
int pesquisa(int*, int, int, Item *, FILE *, int*);

# endif