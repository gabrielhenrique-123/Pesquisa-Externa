#ifndef arvorebe_h
#define arvorebe_h

#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MM 4
#define M 2

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPaginaBE* TipoApontadorBE;

typedef struct TipoPaginaBE {
   TipoIntExt Pt;
   union{
        struct {
            int ni;
            int ri[MM];
            TipoApontadorBE pi[MM + 1];
        } Interna;
        struct{
            int ne;
            Item re[MM];
        } Externa;
   } Pagina;
} TipoPaginaBE;

//Inicializar
void InicializaArvoreBstar(TipoApontadorBE *Ap);


//Pesquisa
void PesquisaBE (Item*, TipoApontadorBE*, int*);

//Inserção

void InsereNaPaginaBstarExterna(TipoApontadorBE Ap, Item Reg, int*);
void InsereNaPaginaBstarInterna(TipoApontadorBE Ap, int chave, TipoApontadorBE ApDir, int*);
void InsereBstar(Item Reg, TipoApontadorBE *Ap, int*);
//ins
void InsereNaPaginaBstar(TipoApontadorBE Ap, Item Reg, short *Cresceu, Item *RegRetorno, TipoApontadorBE *ApRetorno, int*);


#endif 