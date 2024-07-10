#include "arvoreb.h"
#include "item.h"

/*void Pesquisa(TipoRegistro *x, TipoApontador Ap){
    long i = 1;

    if(Ap == NULL){
        printf("TipoRegistro nao existe\n");
        return
    }

    while(i < Ap->n && x->chave > Ap->r[i-1].chave)
        i++;

    if(Ap->r[i-1].chave == x->chave){
        *x = Ap->r[i-1];
        return;
    } 

    if(x->chave < Ap->r[i-1].chave)
        Pesquisa(x, Ap->p[i-1]);

        else
            Pesquisa(x, Ap->p[i]);
}
*/


//****************************************Inicialização: Arvore B*****************************************************************

TipoApontador Inicializa (){
    TipoApontador raiz = (TipoApontador)malloc(sizeof(TipoPagina));
    raiz->n = 0;

    //Inicialização dos dados da raiz
    for (int i = 0; i < MM; i++) {
        raiz->r[i].chave = -1;
        raiz->r[i].dado1 = -1;
        raiz->r[i].dado2[0] = '\0';
        raiz->r[i].dado3[0] = '\0';
    }
    
    //Faz todos os ponteiros da raiz apontarem para o nulo
    for (int i = 0; i < MM + 1; i++) {
        raiz->p[i] = NULL;
    }
    
    return raiz;
}

//******************************************Pesquisa: Arvore B *****************************************************************

void Pesquisa(Item *x, TipoApontador Ap, int *comp){
    long i = 1;

    if (Ap == NULL) {
        return;
    }

    // Pesquisa sequencial para se encontrar o intervalo desejado 
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) {
        i++; 
        *comp += 1;
    }

    *comp += 1;
    //Verifica se a chave desejada foi localizada
    if (x->chave == Ap->r[i-1].chave) {
        *x = Ap->r[i-1];
        return;
    }

    *comp += 1;
    if (x->chave < Ap->r[i-1].chave){   //Se a chave desejada for menor que a chave atual, deve ir para esquerda, que por convenção tem os menores
        Pesquisa(x, Ap->p[i-1], comp); // Ativação recursiva da Pesquisa na subárvore esquerda
    }

    else{       // Se a chave desejada for maior que a chave atual, deve ir para direita, que por convenção tem os maiores
        Pesquisa (x, Ap->p[i], comp); //Ativação recursiva da Pesquisa na subárvore direita
    }
}

//**********************************************************Inserção: Arvore B************************************************************

void InsereNaPagina (TipoApontador Ap, Item Reg, TipoApontador ApDir, int *comp) {
    short NaoAchouPosicao;
    int k;

    k = Ap->n;      

    NaoAchouPosicao = (k>0);

    //So entra no while caso nao ache posicao
    while (NaoAchouPosicao) {
        *comp += 1;
        if (Reg.chave >= Ap->r[k-1].chave) {
            NaoAchouPosicao = false;
            break;
        }

        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;

        if(k<1){ 
            NaoAchouPosicao = false;
        }

    }

    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(Item Reg, TipoApontador Ap, short *Cresceu, Item *RegRetorno, TipoApontador *ApRetorno, int *comp) {
    long i = 1; 
    long j;
    TipoApontador ApTemp;

    //Caso a árvore esteja vazia entra no if
    if(Ap == NULL) {
        *Cresceu = true;
        (*RegRetorno) = Reg; 
        (*ApRetorno) = NULL;
        return;
    }

    while (i<Ap->n && Reg.chave > Ap->r[i-1].chave){ 
        i++;
        *comp += 1;
    }

    *comp += 1;
    
    if(Reg.chave == Ap->r[i-1].chave){
        printf("Erro: Registro já está presente\n");
        *Cresceu = false;
        return;
    }

    *comp += 1;
    
    //Caso queira caminhar para esquerda, senão, irá caminhar para a direita
    if(Reg.chave < Ap->r[i-1].chave) 
        i--;    //Transforma i em 0
    
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, comp); //Se 1, vai para a direita, se 0 para a esquerda

    if(!*Cresceu) 
        return;

    if(Ap->n < MM) { //pagina tem espaço
        InsereNaPagina (Ap, *RegRetorno, *ApRetorno, comp);
        *Cresceu = false;
        return;
    }

    //Overflow: Pagina tem que ser dividida
    //As primeiras 3 linhas focam na criação de nova página
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    
    if(i<M+1){      //Elemento será inserido na página existente
        InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM], comp);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, comp);
    }

    else    //Será inserido na nova página
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, comp);
    
    for(j=M+2; j<= MM; j++)     //Atualizações finais na página
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j], comp);

    Ap->n = M; 
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M]; //Retorna o valor do meio para subir
    *ApRetorno = ApTemp;
}

void Insere(Item Reg, TipoApontador *Ap, int *comp) {
    short Cresceu;  //Indica se árvore cresceu
    Item RegRetorno;    
    TipoPagina *ApRetorno, *ApTemp;

    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, comp);   //Reg = Qual registro sera inserido     *Ap = Em qual árvore será inserido      Cresceu = controle para saber se a árvore cresceu para cima
                                                        //RegRetorno = registro que será retornado      ApRetorno = filho à direita do resgistro retornado 

    //Se passar por Ins e Cresceu receber o valor 1, então entra no if
    if(Cresceu) { //Arvore cresce na altura pela raiz
        ApTemp = (TipoPagina*) malloc (sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp; 
    }
    //Todos os passos acima acontecem caso seja preciso criar uma nova raiz
}