#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 257
#define MEGABYTE 8388608

// funções auxiliares

void ReconstroiCodigos(char *cabecalho, int tamCabecalho, int l, int c, char codigos[l][c], char codigo_atual[c], int profundidade, int *posNoCabecalho);
void InverteString(char* str);
int EhIgualCodigo(char *codigo, char *string);
void TransformaCabecalhoEmString(int tamCabecalho, char *cabecalho, bitmap *bm, int *index);
bitmap *LeMegaByteDoArquivo(FILE *arqbin);
void DecodificaTexto(FILE *arqbin, bitmap *bm, int index, int l, int c, char codigos[l][c], FILE *arqsaida, unsigned long int qtdTotalCaracteres);

int main(int argc, char *argv[])
{
    // abrindo arquivos:
    if (argc <= 1)
    {
        printf("ERRO: arquivo compactado não foi informado.\n");
        return EXIT_FAILURE;
    }
    FILE *arqbin = fopen(argv[1], "rb");
    if (arqbin == NULL)
    {
        printf("ERRO: não foi possível ler o arquivo ./%s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char nomeCompleto[101];
    argv[1][strlen(argv[1]) - strlen(".comp")] = '\0'; // tira o '.comp'
    sprintf(nomeCompleto, "%s (descompactado)", argv[1]);
    FILE *arqSaida = fopen(nomeCompleto, "w");
    if (arqSaida == NULL)
    {
        printf("ERRO: não foi possível abrir o arquivo ./%s\n", nomeCompleto);
        return EXIT_FAILURE;
    }

    // -------------------

    // leitura de metadados
    int alturaAbHuff = 0, tamCabecalho = 0;
    unsigned long int qtdTotalCaracteres = 0;
    fread(&qtdTotalCaracteres, sizeof(unsigned long int), 1, arqbin);
    fread(&alturaAbHuff, sizeof(int), 1, arqbin);
    fread(&tamCabecalho, sizeof(int), 1, arqbin);

    printf("Gerando tabela de codificação a partir do cabeçalho...\n");

    bitmap *bm = LeMegaByteDoArquivo(arqbin);

    char cabecalho[tamCabecalho];
    int index = 0; // armazena a posição do bit no bm após a leitura do cabeçalho
    TransformaCabecalhoEmString(tamCabecalho, cabecalho, bm, &index);

    // transforma a string lida do cabeçalho na tabela de codificação
    char codigo_atual[alturaAbHuff];
    char(*codigos)[alturaAbHuff] = calloc(ASCII_SIZE * alturaAbHuff, sizeof(char));
    if (!codigos)
    {
        printf("Problema na alocação de memória.\n");
        return EXIT_FAILURE;
    }
    int pos = 0;
    ReconstroiCodigos(cabecalho, tamCabecalho, ASCII_SIZE, alturaAbHuff, codigos, codigo_atual, 0, &pos);

    // for (int i = 0; i < ASCII_SIZE; i++)
    //     if (codigos[i][0] != '\0')
    //         printf("Caractere (%d): %s\n", i, codigos[i]);

    printf("Tabela de codificação gerada.\n\n");

    // -------------------

    printf("Descompactando...\n");

    DecodificaTexto(arqbin, bm, index, ASCII_SIZE, alturaAbHuff, codigos, arqSaida, qtdTotalCaracteres);

    printf("Arquivo '%s' descompactado com sucesso!\n", argv[1]);
    printf("Arquivo descompactado: '%s'.\n", nomeCompleto);

    // liberações
    free(codigos);
    fclose(arqSaida);
    fclose(arqbin);

    return EXIT_SUCCESS;
}

void InverteString(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

void ReconstroiCodigos(char *cabecalho, int tamCabecalho, int l, int c, char codigos[l][c], char codigo_atual[c], int profundidade, int *posNoCabecalho)
{
    if (*posNoCabecalho == tamCabecalho)
        return;

    if (cabecalho[*posNoCabecalho] == '1')
    {
        (*posNoCabecalho)++;
        codigo_atual[profundidade] = '\0';
        char caracter = cabecalho[(*posNoCabecalho)];
        strcpy(codigos[(unsigned char)caracter], codigo_atual);
        (*posNoCabecalho)++;
    }
    else if (cabecalho[*posNoCabecalho] == '0')
    {
        (*posNoCabecalho)++;
        // manda pra esquerda
        codigo_atual[profundidade] = '0';
        ReconstroiCodigos(cabecalho, tamCabecalho, l, c, codigos, codigo_atual, profundidade + 1, posNoCabecalho);
        // madnda pra direita
        codigo_atual[profundidade] = '1';
        ReconstroiCodigos(cabecalho, tamCabecalho, l, c, codigos, codigo_atual, profundidade + 1, posNoCabecalho);
    }
}

int EhIgualCodigo(char *codigo, char *string)
{
     return !strcmp(codigo, string);
}

void TransformaCabecalhoEmString(int tamCabecalho, char *cabecalho, bitmap *bm, int *index)
{

    int flag = 0, posicaoBm = 0, posicaoString = 0;
    *index = 0;
    unsigned char bitAtual = '\0';

    while (posicaoBm < tamCabecalho)
    {

        bitAtual = bitmapGetBit(bm, posicaoBm) + '0';
        posicaoString++;
        posicaoBm++;

        if (bitAtual == '0')
        {
            cabecalho[posicaoString - 1] = '0'; // representação de nó não folha
            continue;
        }
        else if (bitAtual == '1')
        { // nó folha ou possível condição de parada

            cabecalho[posicaoString - 1] = '1'; // representação do nó folha

            // leitura da letra contida no nó folha
            unsigned char byte = 0x00;
            for (int j = 0; j < 8; j++)
            {
                unsigned char bit = bitmapGetBit(bm, posicaoBm + j);
                byte |= (bit << (7 - j));
            }
            posicaoBm += 8;

            posicaoString++;
            cabecalho[posicaoString - 1] = byte;
        }
    }
    cabecalho[posicaoString] = '\0';
    *index = posicaoBm;
}

bitmap *LeMegaByteDoArquivo(FILE *arqbin)
{
    unsigned char byte;

    bitmap *bm = bitmapInit((unsigned int)MEGABYTE);

    int flagFinaliza = 0;
    while (!flagFinaliza && (fread(&byte, sizeof(unsigned char), 1, arqbin) == 1))
    {
        // escreve o byte bit a bit

        for (int i = 7; i >= 0; i--)
        {
            bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);

            if (bitmapGetLength(bm) == MEGABYTE)
            {
                flagFinaliza = 1;
                break;
            }
        }
    }

    return bm;
}

void DecodificaTexto(FILE *arqbin, bitmap *bm, int index, int l, int c, char codigos[l][c], FILE *arqsaida, unsigned long int qtdTotalCaracteres)
{
    char codigoAtual[c];
    int j = 0;
    unsigned long int qtdCaracteresDecodificados = 0;

    while (qtdCaracteresDecodificados < qtdTotalCaracteres)
    {
        if (index == MEGABYTE)
        {
            bitmapLibera(bm);
            bm = LeMegaByteDoArquivo(arqbin);
            index = 0;
        }

        codigoAtual[j] = bitmapGetBit(bm, index) + '0';
        codigoAtual[j + 1] = '\0';

        j++;
        index++;

        for (size_t i = 0; i < ASCII_SIZE; i++)
        {
            if (EhIgualCodigo(codigoAtual, codigos[i]))
            {
                qtdCaracteresDecodificados++;
                fputc(i, arqsaida);
                j = 0;
            }
        }
    }
    bitmapLibera(bm);
}

