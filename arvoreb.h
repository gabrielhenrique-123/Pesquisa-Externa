#ifndef arvoreb_h
#define arvoreb_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "item.h"

#define MM 4
#define M 2

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n;
    Item r[MM];
    TipoApontador p[MM +1];
} TipoPagina;

//Inicializar
TipoApontador Inicializa (); 


//Pesquisa
void Pesquisa (Item*, TipoApontador, int*);

//Caminhamento
void Imprime (TipoApontador);

//Inserção
void InsereNaPagina (TipoApontador, Item, TipoApontador, int*);
void Ins(Item, TipoApontador, short*, Item*, TipoApontador*, int*);
void Insere(Item, TipoApontador*, int*);


//Remoção
void Reconstitui (TipoApontador, TipoApontador, int, short*);
void Antecessor (TipoApontador, int, TipoApontador, short*);
void Ret(int, TipoApontador*, short*);
void Retira (int, TipoApontador*);

#endif 