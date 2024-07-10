#include "ASI.h"

int acessoSequencialIndexado( Item *itemProcurado, FILE * arq, Analise *analise){
    int tamanhoTabela, contador = 0, comp = 0, transf = 0;
    
    // verificar o número total de itens do arquivo
    clock_t startI = clock();
    
    fseek(arq, 0, SEEK_END);
    int totalItens = ftell(arq)/ sizeof(Item);
    
    tamanhoTabela = totalItens / ITENSPORPAGINA;
    
    if (totalItens % ITENSPORPAGINA != 0){
        tamanhoTabela++;
    }
    
    int *tabela = malloc(sizeof(int) * tamanhoTabela);
    
    Item x[ITENSPORPAGINA];
    rewind(arq);
    
    // cria o vetor de indices
    while(fread(x, sizeof(Item), ITENSPORPAGINA, arq) > 0){
        tabela[contador] = x[0].chave;
        contador++;
        transf++;
    }
    clock_t endI = clock();

    analise->insercaoTemp = (endI - startI);
    analise->insercaoComp = 0;
    analise->insercaoTransf = transf;

    comp = 0;
    transf = 0;
    
    clock_t startP = clock();
    pesquisa(tabela, totalItens, tamanhoTabela, itemProcurado, arq, &comp);
    clock_t endP = clock();

    analise->pesquisaTemp = (endP - startP);
    analise->pesquisaTransf = 0;
    analise->pesquisaComp = comp;

    fclose(arq);
    free(tabela);
    return 1;
}

int pesquisa(int tabela[], int totalItens, int tamanhoTabela, Item *itemProcurado, FILE * arq, int *comp){
    int desloc, quantidade, i = 0;
    Item pagina[ITENSPORPAGINA];
    
    // verifica a pagina que o item procurado pode estar
    while(i <= tamanhoTabela && itemProcurado->chave >= tabela[i]){ 
        i++;
        *comp += 1;
    }
    
    if(i==0) 
        return  0;
    
    // verifica se esta na ultima pag e ve a qntd de itens nela 
    if(i < tamanhoTabela || totalItens % ITENSPORPAGINA == 0) 
        quantidade = ITENSPORPAGINA;
    else{
        quantidade = totalItens % ITENSPORPAGINA;
    }
    
    // desloca o ponteiro para a página
    if(i < tamanhoTabela) 
        desloc = (i-1) * ITENSPORPAGINA * sizeof(Item);
    
    else 
        desloc = (tamanhoTabela - 1) * ITENSPORPAGINA * sizeof(Item);
    
    fseek(arq, desloc, 0);
    fread(&pagina, sizeof(Item), quantidade, arq);
    
    // procura o item na página
    for(i = 0; i < quantidade; i++){
        if (pagina[i].chave == itemProcurado->chave){
            *itemProcurado = pagina[i];
            *comp += 1;
            return 1;
        }
    }
    return 0;
}