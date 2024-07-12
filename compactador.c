#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 128
#define MEGABYTE 8388608

lista* FazListaHuff(int* pesos) {
    lista* l = CriaLista();
    node* a = NULL;
    for(int i = 0; i < ASCII_SIZE; i++) {
        if(pesos[i]) {
            a = CriaArvore(i, pesos[i], CriaVazio(), CriaVazio());
            InsereLista(l, a);
        }
    }
    // cria caractere ETX para comunicar o fim do texto
    a = CriaArvore(3, 1, CriaVazio(), CriaVazio());
    InsereLista(l, a);
    
    return l;
}

int main(int argc, char *argv[])
{
    // arquivo de entrada
    if(argc <= 1) {
        printf("ERRO: arquivo a ser compactado não foi informado.\n");
        return EXIT_FAILURE;
    }
    FILE* arqtxt = fopen(argv[1], "r");
    if(arqtxt == NULL) {
        printf("ERRO: não foi possível ler o arquivo ./%s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // array de ints representando a tabela ASCII que contem os pesos de cada letra
    int pesos[ASCII_SIZE] = {0};
    // le o input e determina os pesos de cada letra
    int caractere = 0;
    while ((caractere = fgetc(arqtxt)) != EOF) pesos[caractere]++;

    lista *listaArvores = FazListaHuff(pesos);
    node* abHuff = CriaArvoreHuff(listaArvores);
    int alturaAbHuff = RetornaAlturaArvore(abHuff);
    // printf("altura: %d\n", alturaAbHuff);


    // arquivo de saida compactado
    char nome[31];
    sprintf(nome, "%s.comp", argv[1]);
    FILE* arqbin = fopen(nome, "wb");
    if(arqbin == NULL) {
        printf("ERRO: não foi possível abrir o arquivo ./%s\n", nome);
        return EXIT_FAILURE;
    }

    fwrite(&alturaAbHuff, sizeof(int), 1, arqbin);

    char codigoAtual[alturaAbHuff];
    char (*codigos)[alturaAbHuff] = calloc (ASCII_SIZE*alturaAbHuff, sizeof(char));

    // gera os codigos binarios para cada caractere no texto
    GeraCodigos(abHuff, codigoAtual, 0, ASCII_SIZE, alturaAbHuff, codigos);

    // busca o fim do arquivo para saber o tamanho dele
    fseek(arqtxt, 0, SEEK_END);

    long size = ftell(arqtxt);

    // printa os codigos gerados
    // printf("\n");
    // for (int i = 0; i < ASCII_SIZE; i++)
    //     if (codigos[i][0] != '\0') printf("Caractere %c(%d): %s\n", i, i, codigos[i]);

    
    bitmap *bm = bitmapInit(MEGABYTE);
    // escrevre a arvore de Huffman no bm
    EscreveCabecalho(bm, abHuff);

    rewind(arqtxt);

    while ((caractere = fgetc(arqtxt)) != EOF) {

        // escreve cada bit do codigo de um caracter no bitmap
        for (size_t j = 0; codigos[caractere][j] != '\0'; j++) {
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[caractere][j]);
            if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)) {
                fwrite(bitmapGetContents(bm), bitmapGetLength(bm)/8, 1, arqbin);
                bitmapLibera(bm);
                bm = bitmapInit(MEGABYTE);
            }
        }
    }
    // coloca o caractere de final
    for (size_t j = 0; codigos[3][j] != '\0'; j++)
        bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[3][j]);

    fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

    LiberaArvore(abHuff);
    free(codigos);
    bitmapLibera(bm);
    fclose(arqbin);
    fclose(arqtxt);
    return EXIT_SUCCESS;
}