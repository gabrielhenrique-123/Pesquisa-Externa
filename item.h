#ifndef item_h
#define item_h

typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
}Item;


typedef struct Analise {
    int insercaoComp, pesquisaComp, insercaoTransf, pesquisaTransf;
    double insercaoTemp, pesquisaTemp;
} Analise;

# endif