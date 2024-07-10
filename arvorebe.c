#include "arvorebe.h"
#include "item.h"

// Inicialização: Arvore B*

void InicializaArvoreBstar(TipoApontadorBE *Ap){
    //Inicializa a primeira página, sendo ela externa
    (*Ap) = (TipoPaginaBE*) malloc (sizeof(TipoPaginaBE));
    (*Ap)->Pt = Externa;        //Definindo o tipo da primeira pagina
    (*Ap)->Pagina.Externa.ne = 0;   //Numero de itens na pagina
    (*Ap)->Pagina.Externa.re[0].chave = -1;  //Primeira chave no registro posição 0

}

/*void PesquisaBE(TipoRegistro *x, TipoApontador *Ap){
    long i = 1;

    TipoRegistro Pag;
    Pag = *Ap;

    if((*Ap)->Pt == Interna){
        while(i < Pag->)
    }

}
*/
// Pesquisa: Arvore B*
void PesquisaBE(Item *x, TipoApontadorBE *Ap, int *comp){
    int i;
    TipoApontadorBE Pag;
    Pag = *Ap;

    if((*Ap)->Pt == Interna){
        i = 1;

        while(i < Pag->Pagina.Interna.ni && x->chave > Pag->Pagina.Interna.ri[i-1]){
            i++;
            *comp += 1;
        }
        
        *comp += 1;
        if(x->chave < Pag->Pagina.Interna.ri[i-1]){
            PesquisaBE(x, &Pag->Pagina.Interna.pi[i-1], comp);
        }
            else{
                PesquisaBE(x, &Pag->Pagina.Interna.pi[i], comp);
            }
        return;
    }
    i = 1;

    //encontra pagina externa
    while(i < Pag->Pagina.Externa.ne && x->chave > Pag->Pagina.Externa.re[i-1].chave){
        i++;
        *comp += 1;
    }

    *comp += 1;
    //Passa o registro por referência caso tenha encontrado
    if(x->chave == Pag->Pagina.Externa.re[i-1].chave){
        *x = Pag->Pagina.Externa.re[i-1];
        return;
    }
        //Caso nao ache
        else {
            return;
        }
}
//insere

void InsereNaPaginaBstarExterna(TipoApontadorBE Ap, Item Reg, int *comp) {
    
    int i = Ap->Pagina.Externa.ne;      //Quantidade de itens na página que será inserido o registro
                                        //Em caso de OVERFLOW, i começará com o valor 0
    short NaoAchouPosicao = (i > 0);    //Variável de controle para identificar se existe uma posição

    //While serve para ordenar chaves dentro da página
    while(NaoAchouPosicao){
        *comp += 1;
        // Numero que será inserido || Numero atual percorrido
        if(Reg.chave >= Ap->Pagina.Externa.re[i-1].chave){
            NaoAchouPosicao = false;
            break;
        }
        Ap->Pagina.Externa.re[i] = Ap->Pagina.Externa.re[i-1];
        i--;

        if(i<1)
            NaoAchouPosicao = false;
    }
    // Declarações finais
    Ap->Pagina.Externa.re[i] = Reg; //Aplica o registro na posição correta na página
    Ap->Pagina.Externa.ne++;    //Adiciona +1 na quantidade de itens na página
}

void InsereNaPaginaBstarInterna(TipoApontadorBE Ap, int chave, TipoApontadorBE ApDir, int *comp) {
    int i = Ap->Pagina.Interna.ni;
    short NaoAchouPosicao = (i > 0);

    while(NaoAchouPosicao){
        *comp += 1;
        if(chave >= Ap->Pagina.Interna.ri[i-1]){
            NaoAchouPosicao = false;
            break;
        }
        Ap->Pagina.Interna.ri[i] = Ap->Pagina.Interna.ri[i-1];
        Ap->Pagina.Interna.pi[i+1] = Ap->Pagina.Interna.pi[i];
        i--;

        if(i < 1)
            NaoAchouPosicao = false;
    }


    Ap->Pagina.Interna.ri[i] = chave;
    Ap->Pagina.Interna.pi[i + 1] = ApDir;
    Ap->Pagina.Interna.ni++;
}

void InsereBstar(Item Reg, TipoApontadorBE *Ap, int *comp) {
    short Cresceu;          //Valor true ou false para fazer o controle de crescimento da arovre
    Item RegRetorno;
    TipoPaginaBE *ApRetorno, *ApTemp;

    InsereNaPaginaBstar(*Ap, Reg, &Cresceu, &RegRetorno, &ApRetorno, comp);   // *Ap = em qual árvore será inserido       Reg = registro a ser inserido       RegRetorno = registro retornado
                                                                        // ApRetorno = filho à direita do registro retornado
    //Se arvore crescer na altura pela raiz
    if (Cresceu) {  //Cresceu ter que ser true
        ApTemp = (TipoPaginaBE*) malloc(sizeof(TipoPaginaBE));
        ApTemp->Pt = Interna;
        ApTemp->Pagina.Interna.ni = 1;
        ApTemp->Pagina.Interna.ri[0] = RegRetorno.chave;
        ApTemp->Pagina.Interna.pi[1] = ApRetorno;
        ApTemp->Pagina.Interna.pi[0] = *Ap;
        *Ap = ApTemp;
    }
}

void InsereNaPaginaBstar(TipoApontadorBE Ap, Item Reg, short *Cresceu, Item *RegRetorno, TipoApontadorBE *ApRetorno, int *comp) {
    long i = 1, j;
    TipoApontadorBE ApTemp;

    //Caso a página recebida seja externa, ou seja, está nos nós folha
    if(Ap->Pt == Externa){
        
        //Condicional para ver se a página externa está cheia
        //Overflow!!!!
        if(Ap->Pagina.Externa.ne == MM){
            ApTemp = (TipoApontadorBE) malloc(sizeof(TipoPaginaBE));    //Cria uma nova página
            ApTemp->Pt = Externa;       //Coloca ela como externa
            ApTemp->Pagina.Externa.ne = 0;      

            if(i < M + 1){
                //ApTempo = Nova página     //Ap->Pagina.Externa.re[MM-1] = Ultimo registro da pagina ja existente que Ap aponta
                InsereNaPaginaBstarExterna(ApTemp, Ap->Pagina.Externa.re[MM-1], comp);    //Passa o ultimo item da pagina, para a nova pagina
              
                Ap->Pagina.Externa.ne--;   //Tira a referência do ultimo item da pagina ja existente, transformando numa pagina de tamanho 3, não mais 4

                InsereNaPaginaBstarExterna(Ap, Reg, comp);
            }
            else
                InsereNaPaginaBstarExterna(ApTemp, Reg, comp);
           
            for(j = M+2; j <= MM; j++)          //Atualizações finais na página
                InsereNaPaginaBstarExterna(ApTemp, Ap->Pagina.Externa.re[j-1], comp);
            

            Ap->Pagina.Externa.ne = M+1;
            *RegRetorno = Ap->Pagina.Externa.re[M];
            *ApRetorno = ApTemp;
            *Cresceu = true;
            return ;
        }
        //Se tiver espaço na página
        else{
            InsereNaPaginaBstarExterna(Ap, Reg, comp);
            *Cresceu = false;
            return ;
        }
    }
    //PAGINAS INTERNAS
    else{
        while(i < Ap->Pagina.Interna.ni && Reg.chave > Ap->Pagina.Interna.ri[i-1]){ 
            i++;
            *comp += 1;
        }

        *comp += 1;

        if(Reg.chave == Ap->Pagina.Interna.ri[i-1]){
            printf("Erro: Registro ja esta presente");
            *Cresceu = false;
            return;
        }

        *comp += 1;

        if(Reg.chave < Ap->Pagina.Interna.ri[i-1])
            i--;
        
        InsereNaPaginaBstar(Ap->Pagina.Interna.pi[i], Reg, Cresceu, RegRetorno, ApRetorno, comp);

        if(!*Cresceu) 
            return ;

        if(Ap->Pagina.Interna.ni < MM){
            InsereNaPaginaBstarInterna(Ap, RegRetorno->chave, *ApRetorno, comp);
            *Cresceu = false;
            return ;
        }

        ApTemp = (TipoApontadorBE) malloc(sizeof(TipoPaginaBE));
        ApTemp->Pt = Interna;
        ApTemp->Pagina.Interna.ni = 0;
        ApTemp->Pagina.Interna.pi[0] = NULL;

        if(i < M + 1){
            InsereNaPaginaBstarInterna(ApTemp, Ap->Pagina.Interna.ri[MM-1], Ap->Pagina.Interna.pi[MM], comp);
            Ap->Pagina.Interna.ni--;
            InsereNaPaginaBstarInterna(Ap, RegRetorno->chave, *ApRetorno, comp);
        }
        else{
            InsereNaPaginaBstarInterna(ApTemp, RegRetorno->chave, *ApRetorno, comp);
        }

        for(j = M+2; j <= MM; j++)
            InsereNaPaginaBstarInterna(ApTemp, Ap->Pagina.Interna.ri[j-1], Ap->Pagina.Interna.pi[j], comp);

        Ap->Pagina.Interna.ni = M;
        ApTemp->Pagina.Interna.pi[0] = Ap->Pagina.Interna.pi[M+1];
        RegRetorno->chave = Ap->Pagina.Interna.ri[M];
        *ApRetorno = ApTemp;
    }
}
