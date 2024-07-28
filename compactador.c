#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 257
#define MEGABYTE 8388608

// função auxiliar

lista* FazListaHuff(int* pesos);

int main(int argc, char *argv[])
{
    // arquivo de entrada:
    if(argc <= 1) {
        printf("ERRO: arquivo a ser compactado não foi informado.\n");
        return EXIT_FAILURE;
    }
    FILE* arqSaida = fopen(argv[1], "r");
    if(arqSaida == NULL) {
        printf("ERRO: não foi possível ler o arquivo %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // arquivo de saída:
    char nome[31];
    sprintf(nome, "%s.comp", argv[1]);
    FILE* arqbin = fopen(nome, "wb");
    if(arqbin == NULL) {
        printf("ERRO: não foi possível abrir o arquivo %s\n", nome);
        return EXIT_FAILURE;
    }
    // -----------------

    unsigned long int tamArqBinBytes = 2*sizeof(int) + sizeof(unsigned long int); // tamanho dos metadados

    unsigned long int qtdTotalCaracteres = 0;
    fwrite(&qtdTotalCaracteres, sizeof(unsigned long int), 1, arqbin); // já reserva o espaço no arquivo
    // o valor real será calculado ao longo do programa, sendo reescrito após o cálculo


    printf("Gerando árvore ótima de Huffman...\n");

    // array de ints representando a tabela ASCII que contém os pesos de cada letra
    int pesos[ASCII_SIZE] = {0};

    // lê o arquivo de entrada e determina os pesos de cada letra
    int caractere = 0;
    while ((caractere = fgetc(arqSaida)) != EOF) pesos[caractere]++;

    lista *listaArvores = FazListaHuff(pesos);
    arvBin* abHuff = CriaArvoreHuff(listaArvores);
    int alturaAbHuff = RetornaAlturaArvore(abHuff);

    printf("Árvore criada.\n\n");
    fwrite(&alturaAbHuff, sizeof(int), 1, arqbin);

    // -----------------

    printf("Criando tabela de codificação...\n");

    char codigoAtual[alturaAbHuff];
    char (*codigos)[alturaAbHuff] = calloc(ASCII_SIZE*alturaAbHuff, sizeof(char));

    GeraCodigos(abHuff, codigoAtual, 0, ASCII_SIZE, alturaAbHuff, codigos);

    // for (int i = 0; i < ASCII_SIZE; i++)
    //     if (codigos[i][0] != '\0') printf("Caractere (%d): %s\n", i, codigos[i]);

    printf("Tabela de codificação criada.\n\n");
    
    // -----------------

    printf("Compactando...\n");

    bitmap *bm = bitmapInit(MEGABYTE);

    // escrevre a árvore de Huffman no início do bm
    EscreveArvoreBitmap(bm, abHuff);
    int tamCabecalho = bitmapGetLength(bm);
    fwrite(&tamCabecalho, sizeof(int), 1, arqbin);

    rewind(arqSaida);

    while ((caractere = fgetc(arqSaida)) != EOF) {
        // escreve cada bit do código de um caractere no bitmap
        qtdTotalCaracteres++;
        for (size_t j = 0; codigos[caractere][j] != '\0'; j++) {
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[caractere][j]);
            if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)) {
                tamArqBinBytes += (bitmapGetMaxSize(bm)/8);
                fwrite(bitmapGetContents(bm), bitmapGetMaxSize(bm)/8, 1, arqbin);
                bitmapLibera(bm);
                bm = bitmapInit(MEGABYTE);
            }
        }
    }

    // escreve o conteúdo restante do bm (caso exista) no arquivo binário
    if(bitmapGetLength(bm)) {
        tamArqBinBytes += (bitmapGetLength(bm) + 7) / 8;
        fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);
    }

    // volta para o início do arquivo binário e escreve o valor real da quantidade de caracteres
    rewind(arqbin);
    fwrite(&qtdTotalCaracteres, sizeof(unsigned long int), 1, arqbin);

    printf("Arquivo '%s' compactado com sucesso!\n", argv[1]);
    printf("Arquivo compactado: '%s'\n\n", nome);

    unsigned long int tamArqOriginalBytes = qtdTotalCaracteres*sizeof(char);
    printf("Tamanho do arquivo original: %ld bytes.\n", tamArqOriginalBytes);
    printf("Tamanho do arquivo compactado: %ld bytes.\n", tamArqBinBytes);

    if(tamArqBinBytes >= tamArqOriginalBytes) printf("Não houve redução no tamanho do arquivo.\n");
    else {
        float red = 100.00*((tamArqOriginalBytes - tamArqBinBytes)/(float)tamArqOriginalBytes);
        printf("Redução em, aproximadamente, %.02f%%.\n", red);
    }
    

    // liberações
    LiberaArvore(abHuff);
    free(codigos);
    bitmapLibera(bm);
    fclose(arqbin);
    fclose(arqSaida);
    
    return EXIT_SUCCESS;
}

lista* FazListaHuff(int* pesos) {
    lista* l = CriaLista();
    arvBin* a = NULL;
    for(int i = 0; i < ASCII_SIZE; i++) {
        if(pesos[i]) {
            a = CriaArvore(i, pesos[i], CriaArvoreVazia(), CriaArvoreVazia());
            InsereLista(l, a);
        }
    }
    
    return l;
}