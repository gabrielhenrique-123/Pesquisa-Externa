#include <stdio.h>
#include <string.h>
#include "item.h"
#include "ASI.h"
#include "arvoreBinaria.h"
#include "criaArq.h"
#include "arvoreb.h"
#include "arvorebe.h"

int main(int argc, char **argv)
{
    Item itemProcurado;
    Analise analise;
    //Número de argumentos inválido
    if (argc!=5 && argc!=6){
        printf("Número de argumentos inválido. As entradas são: <método> <quantidade> <situação> <chave> [-P](opcional)\n");
        return 0;
    }
    int opcode = atoi(argv[1]);
    int qtdRegistros = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    itemProcurado.chave = atoi(argv[4]);
    bool ArgOpcional = false;

    if(argc == 6 && strcmp(argv[5], "-p") == 0)
        ArgOpcional = true;

    /*  argv[1] = método - acesso sequencial, binario, arvore b, arvore b estrela
        argv[2] = quantidade de registros
        argv[3] = situação (ordenado descententemente, ascendentemente ou desordenado aleatoriamente)
        argv[4] = chave a ser pesquisada
        argv[5](opcional) = [-P] colocado caso deseja que as chaves de pesquisa dos registros do arquivo considerado sejam apresentadas na tela
    */
    arquivo(qtdRegistros, situacao);
    FILE *arq;
    if((arq = fopen("arquivo.bin", "rb")) == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }
    //**********************************************************  PESQUISA DE ACESSO INDEXADO:  *****************************************************
    if (opcode == 1)  { 
        Item Reg;

       if(ArgOpcional){ 
            for (int i = 0; i < qtdRegistros; i++){
                fread(&Reg, sizeof(Reg), 1, arq); 
                printf("Chave %d = %d\n", i, Reg.chave);
            }
       }
        if(acessoSequencialIndexado(&itemProcurado, arq, &analise))
            printf("\nItem: %d \nDado 1: %ld\nDado 2: %s\nDado 3: %s\n", itemProcurado.chave, itemProcurado.dado1, itemProcurado.dado2, itemProcurado.dado3);
        else 
            printf("O item não está no arquivo\n");

        printf("\n**********************\n\nDADOS DE ANALISE:\n\n");
        printf("INSERCAO\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n",analise.insercaoTemp, analise.insercaoComp, analise.insercaoTransf);
        printf("PESQUISA\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n", analise.pesquisaTemp, analise.pesquisaComp, analise.pesquisaTransf);
    
        return 0;
    }
    //****************************************************************  ARVORE BINÁRIA  *************************************************************
    else if(opcode == 2){
        int comparacaoIns = 0;
        int transferenciaIns = 0;
        double tempoIns = 0;

        int comparacaoSearch = 0;
        int transferenciaSearch = 0;
        double tempoSearch = 0;

        if(binaryTree(arq, qtdRegistros, &itemProcurado, ArgOpcional, &comparacaoIns, &transferenciaIns, &tempoIns, &comparacaoSearch, &transferenciaSearch, &tempoSearch)){
            printf("Item: %d \nDado 1: %ld\nDado 2: %s\nDado 3: %s\n", itemProcurado.chave, itemProcurado.dado1, itemProcurado.dado2, itemProcurado.dado3);
            printf("\n********\n\nDADOS DE ANALISE:\n\n");
            printf("INSERCAO\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n", tempoIns, comparacaoIns, transferenciaIns);
            printf("PESQUISA\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n", tempoSearch, comparacaoSearch, transferenciaSearch);
        }
        else 
            printf("O item não está no arquivo\n");
        return 0;
    }
    //******************************************************************   ARVORE B   ****************************************************************
    else if (opcode == 3)  {
        Item Reg;
        int transf = 0;     //Transferencias
        int comp = 0;       //Comparacoes
        //Inicialização
        TipoApontador Arvoreb = Inicializa();

        //Inserção
        clock_t startI = clock();
        for (int i = 0; i < qtdRegistros; i++){
            fread(&Reg, sizeof(Reg), 1, arq);
            if(ArgOpcional){ 
                printf("Chave %d = %d\n", i, Reg.chave);
            }
            Insere(Reg, &Arvoreb, &comp);
            transf++;
        }
        clock_t endI = clock();
        
        analise.insercaoTemp = (endI - startI);     //Tempo de insercao 
        analise.insercaoTransf = transf;    //Numero de transferencias da insercao
        analise.insercaoComp = comp;

        comp = 0;   //Reinicia a variável comp
        
        clock_t startP = clock();
        //Pesquisa
        Pesquisa(&itemProcurado, Arvoreb, &comp);    
        clock_t endP = clock();

        if(itemProcurado.chave <= qtdRegistros && itemProcurado.chave > 0)
            printf("\n\nChave: %d \nDado 1: %ld\nDado 2: %s\nDado 3: %s\n", itemProcurado.chave, itemProcurado.dado1, itemProcurado.dado2, itemProcurado.dado3);
        
        else
            printf("A chave desejada nao foi encontrada\n");

        if (argc==5){ //Usuario digitou -P
        }

        analise.pesquisaComp = comp;    //Numero de comparacoes na pesquisa
        analise.pesquisaTemp = (endP - startP); //Tempo de pesquisa
        analise.pesquisaTransf = 0; //Por padrao, sempre 0

        printf("\n**********************\n\nDADOS DE ANALISE:\n\n");
        printf("INSERCAO\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n",analise.insercaoTemp, analise.insercaoComp, analise.insercaoTransf);
        printf("PESQUISA\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n", analise.pesquisaTemp, analise.pesquisaComp, analise.pesquisaTransf);
        free(Arvoreb);
    }
    //*******************************************************************  ARVORE B*   *****************************************************************
    else if (opcode == 4){
        TipoApontadorBE arvorebe;
        Item registro;
        int transf = 0;
        int comp = 0;
        
        //Inicialização

        InicializaArvoreBstar(&arvorebe);

        //Inserção
        clock_t startIBE = clock();
        for(int i=0; i<qtdRegistros; i++){
            fread(&registro, sizeof(registro), 1, arq);
            if(ArgOpcional){ 
                printf("Chave %d = %d\n", i, registro.chave);
            }
            InsereBstar(registro, &arvorebe, &comp);
            transf++;
        }
        clock_t endIBE = clock();

        analise.insercaoTemp = (endIBE - startIBE);
        analise.insercaoComp = comp;
        analise.insercaoTransf = transf;

        registro.chave = atoi(argv[4]);

        comp = 0;

        //Pesquisa
        clock_t startPBE = clock();
        PesquisaBE(&registro, &arvorebe, &comp);
        clock_t endPBE = clock();
        
        if(registro.chave <= qtdRegistros && registro.chave > 0)
            printf("\n\nChave: %d \nDado 1: %ld\nDado 2: %s\nDado 3: %s\n", registro.chave, registro.dado1, registro.dado2, registro.dado3);
        else
            printf("A chave desejada nao foi encontrada");

        analise.pesquisaTemp = (endPBE - startPBE);
        analise.pesquisaTransf = 0; //Sempre 0
        analise.pesquisaComp = comp;

        printf("\n**********************\n\nDADOS DE ANALISE:\n\n");
        printf("INSERCAO\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n",analise.insercaoTemp, analise.insercaoComp, analise.insercaoTransf);
        printf("PESQUISA\nTempo: %lf\nComparacoes: %d\nTransferencias: %d\n\n", analise.pesquisaTemp, analise.pesquisaComp, analise.pesquisaTransf);

        free(arvorebe);    
    }
    
    fclose(arq);
    return 0;
}