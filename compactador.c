#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 257
#define MEGABYTE 8388608

lista* FazListaHuff(int* pesos) {
    lista* l = CriaLista();
    arvBin* a = NULL;
    for(int i = 0; i < ASCII_SIZE; i++) {
        if(pesos[i]) {
            a = CriaArvore(i, pesos[i], CriaArvoreVazia(), CriaArvoreVazia());
            InsereLista(l, a);
        }
    }
    // cria caractere de número 129 (caractere não utilizado)
    a = CriaArvore(3, 1, CriaArvoreVazia(), CriaArvoreVazia());
    InsereLista(l, a);
    
    return l;
}

int main(int argc, char *argv[])
{
    // int a = (int)'—';
    // printf("'—': %d\n", a);
    // return 0;
    
    // arquivo de entrada:
    if(argc <= 1) {
        printf("ERRO: arquivo a ser compactado não foi informado.\n");
        return EXIT_FAILURE;
    }
    FILE* arqtxt = fopen(argv[1], "r");
    if(arqtxt == NULL) {
        printf("ERRO: não foi possível ler o arquivo ./%s\n", argv[1]);
        return EXIT_FAILURE;
    }
    // arquivo de saída:
    char nome[31];
    sprintf(nome, "%s.comp", argv[1]);
    FILE* arqbin = fopen(nome, "wb");
    if(arqbin == NULL) {
        printf("ERRO: não foi possível abrir o arquivo ./%s\n", nome);
        return EXIT_FAILURE;
    }
    // -----------------

    printf("Gerando árvore ótima de Huffman...\n");

    // array de ints representando a tabela ASCII que contém os pesos de cada letra
    int pesos[ASCII_SIZE] = {0};

    // lê o input e determina os pesos de cada letra
    int caractere = 0;
    while ((caractere = fgetc(arqtxt)) != EOF) pesos[caractere]++;

    lista *listaArvores = FazListaHuff(pesos);
    arvBin* abHuff = CriaArvoreHuff(listaArvores);
    int alturaAbHuff = RetornaAlturaArvore(abHuff);

    printf("Árvore criada.\n\n");
    fwrite(&alturaAbHuff, sizeof(int), 1, arqbin);

    // -----------------

    printf("Criando tabela de codificação...\n");

    char codigoAtual[alturaAbHuff];
    char (*codigos)[alturaAbHuff] = calloc (ASCII_SIZE*alturaAbHuff, sizeof(char));

    // gera os códigos binários para cada caractere no texto
    GeraCodigos(abHuff, codigoAtual, 0, ASCII_SIZE, alturaAbHuff, codigos);

    for (int i = 0; i < ASCII_SIZE; i++)
        if (codigos[i][0] != '\0') printf("Caractere (%d): %s\n", i, codigos[i]);

    printf("Tabela de codificação criada.\n\n");
    
    // -----------------

    printf("Compactando...\n");

    bitmap *bm = bitmapInit(MEGABYTE);

    // escrevre a árvore de Huffman no bm
    EscreveCabecalho(bm, abHuff);
    int tamCabecalho = bitmapGetLength(bm);
    fwrite(&tamCabecalho, sizeof(int), 1, arqbin);

    rewind(arqtxt);

    while ((caractere = fgetc(arqtxt)) != EOF) {
        // escreve cada bit do código de um caractere no bitmap
        for (size_t j = 0; codigos[caractere][j] != '\0'; j++) {
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[caractere][j]);
            if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)) {
                fwrite(bitmapGetContents(bm), bitmapGetMaxSize(bm)/8, 1, arqbin);
                bitmapLibera(bm);
                bm = bitmapInit(MEGABYTE);
            }
        }
    }
    // coloca o caractere de final
    for (size_t j = 0; codigos[3][j] != '\0'; j++) {
        bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[3][j]);
        if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)) {
            fwrite(bitmapGetContents(bm), bitmapGetMaxSize(bm)/8, 1, arqbin);
            bitmapLibera(bm);
            bm = bitmapInit(MEGABYTE);
        }
    }

    fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

    printf("Compactação concluída, arquivo '%s' compactado com sucesso!\n", argv[1]);
    printf("Arquivo compactado: %s\n", nome);

    LiberaArvore(abHuff);
    free(codigos);
    bitmapLibera(bm);
    fclose(arqbin);
    fclose(arqtxt);
    
    return EXIT_SUCCESS;
}