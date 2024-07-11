#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 128
#define MEGABYTE 8388608

void LeString(char *frase, int pesos[ASCII_SIZE], lista *lista)
{
    char letra;
    for (int i = 0; frase[i] != '\0'; i++)
    {
        letra = frase[i];
        pesos[letra]++;
    }
}

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

    // arquivo de saida compactado
    char nome[31];
    sprintf(nome, "%s.comp", argv[1]);
    FILE* arqbin = fopen(nome, "wb");
    if(arqbin == NULL) {
        printf("ERRO: não foi possível abrir o arquivo ./%s\n", nome);
        return EXIT_FAILURE;
    }

    long size = ftell(arqtxt);
    char codigoAtual[ASCII_SIZE];
    char codigos[ASCII_SIZE][ASCII_SIZE] = {0};

    // busca o fim do arquivo para saber o tamanho dele
    fseek(arqtxt, 0, SEEK_END);
    

    // gera os codigos binarios para cada caractere no texto
    GeraCodigos(abHuff, codigoAtual, 0, ASCII_SIZE, codigos);

    // printa os codigos gerados
    // printf("\n");
    // for (int i = 0; i < ASCII_SIZE; i++)
    //     if (codigos[i][0] != '\0') printf("Caractere %c(%d): %s\n", i, i, codigos[i]);

    
    bitmap *bm = bitmapInit(MEGABYTE);
    // escrevre a arvore de Huffman no bm
    EscreveCabecalho(bm, abHuff);

    rewind(arqtxt);

    int trocou = 0, trocando = 0;
    while ((caractere = fgetc(arqtxt)) != EOF) {
        trocou = 0;

        if (trocou || trocando)
            printf("caracter procurado: '%c' (%s)\n", caractere, codigos[caractere]);

        if (bitmapGetLength(bm) >= MEGABYTE - 200)
            trocando = 1;

        // escreve cada bit do codigo de um caracter no bitmap
        for (size_t j = 0; codigos[caractere][j] != '\0'; j++)
        {

            // debug
            if (trocando)
            {
                trocando++;
                if (trocando > 200)
                {
                    trocando = 0;
                    printf("--- ACABOU TRECHO ---  \n");
                }
            }
            //  printf("%c", (unsigned char)codigos[caractere][j]);
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[caractere][j]);
            // se chegar perto de do MB, dumpa e cria um novo
        }

        if (bitmapGetLength(bm) >= MEGABYTE - 100)
        {
            printf("TROCOU BM\n");
            fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

            //debugging
            printf(" FIRST CONTENTS - ");
            for (size_t k =  0; k < 30; k++)
            {
                printf("%c", bitmapGetBit(bm,k) + '0');
            }
            printf("\n");

            //libera
            bitmapLibera(bm);
            bm = bitmapInit(MEGABYTE);
            trocou = 1;
        }
    }
    // coloca o caractere de final
    printf("COLOCANDO FINAL\n");
    for (size_t j = 0; codigos[03][j] != '\0'; j++)
    {
        printf("%c", (unsigned char)codigos[03][j]);
        bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[03][j]);
    }

    fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

    LiberaArvore(abHuff);
    bitmapLibera(bm);
    fclose(arqbin);
    fclose(arqtxt);
    return EXIT_SUCCESS;
}