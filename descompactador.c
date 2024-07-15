#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 128
#define MEGABYTE 8388608

void RecontroiCodigos(char *string, int l, int c, char codigos[l][c], char codigo_atual[c], int profundidade, int *pos) {
    if (string[*pos] == '\0') return;

    if (string[*pos] == '1') {
        (*pos)++;
        codigo_atual[profundidade] = '\0';
        char caracter = string[(*pos)];
        strcpy(codigos[(unsigned char)caracter], codigo_atual);
        (*pos)++;
    }
    else if (string[*pos] == '0') {
        (*pos)++;
        // manda pra esquerda
        codigo_atual[profundidade] = '0';
        RecontroiCodigos(string, l, c, codigos, codigo_atual, profundidade + 1, pos);
        // madnda pra direita
        codigo_atual[profundidade] = '1';
        RecontroiCodigos(string, l, c, codigos, codigo_atual, profundidade + 1, pos);
    }
}

int EhIgualCodigo(char *codigo, char *string) {
    return !strcmp(codigo, string);
}

void TransformaCabecalhoEmString(char *string, bitmap *bm, int *index) {

    int flag = 0, posicaoBm = 0, posicaoString = 0;
    *index = 0;
    unsigned char bitAtual = '\0';

    while (1) {
        bitAtual = bitmapGetBit(bm, posicaoBm) + '0';
        //  printf("LI o bit %c\n",bitmapGetBit(bm, i) + '0');
        posicaoString++;
        posicaoBm++;

        if (bitAtual == '0') {
            string[posicaoString - 1] = '0'; // representação de nó não folha
            continue;
        }
        else if (bitAtual == '1') { // nó folha ou possível condição de parada

            if ((bitmapGetBit(bm, posicaoBm) + '0') == '1') {
            // condição de parada: encontrar a sequência '11' (indica o fim da árvore escrita no cabeçalho)
                string[posicaoString] = '\0';
                *index = posicaoBm + 1;
                break;
            }

            string[posicaoString - 1] = '1'; // representação do nó folha

            // leitura da letra contida no nó folha
            unsigned char byte = 0x00;
            for (int j = 0; j < 8; j++) {
                unsigned char bit = bitmapGetBit(bm, posicaoBm + j);
                byte |= (bit << (7 - j));
            }
            posicaoBm += 8;

            posicaoString++;
            string[posicaoString - 1] = byte;
        }
    }
}

bitmap *LeMegaByteDoArquivo(FILE *arqbin) {
    unsigned char byte;

    bitmap *bm = bitmapInit((unsigned int)MEGABYTE);

    int flagFinaliza = 0;
    while (!flagFinaliza && (fread(&byte, sizeof(unsigned char), 1, arqbin) == 1)) {
        //escreve o byte bit a bit

        for (int i = 7; i >= 0; i--) {
            bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);

            if (bitmapGetLength(bm) == MEGABYTE) {
                flagFinaliza = 1;
                break;
            }
        }
    }

    return bm;
}

void DecodificaTexto(FILE* arqbin, bitmap *bm, int index, int l, int c, char codigos[l][c],FILE * arqsaida) {
    char codigo[c];
    int j = 0, found_end_char = 0;

    while (!found_end_char) {
       if (index == MEGABYTE) {
            bitmapLibera(bm);
            bm = LeMegaByteDoArquivo(arqbin);
            index = 0;
        }

        codigo[j] = bitmapGetBit(bm, index) + '0';
        codigo[j + 1] = '\0';

        j++;
        index++;

        for (size_t i = 0; i < ASCII_SIZE; i++) {
            if (EhIgualCodigo(codigo, codigos[i])) {
                if(i == 3) {
                    found_end_char = 1;
                    bitmapLibera(bm);
                    break;
                }
                fputc(i,arqsaida);
                j = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // abrindo arquivos:
    if(argc <= 1) {
        printf("ERRO: arquivo compactado não foi informado.\n");
        return EXIT_FAILURE;
    }
    FILE *arq = fopen(argv[1], "rb");
    if(arq == NULL) {
        printf("ERRO: não foi possível ler o arquivo ./%s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char nome[21];
    sscanf(argv[1], "%[^.]", nome);
    char nomeTxt[51];
    sprintf(nomeTxt, "%sDescompactado.txt", nome);
    FILE *arqtxt = fopen(nomeTxt, "w");
    if(arqtxt == NULL) {
        printf("ERRO: não foi possível abriar o arquivo ./%s\n", nomeTxt);
        return EXIT_FAILURE;
    }
    
    // -------------------

    printf("Gerando tabela de codificação a partir do cabeçalho...\n");

    int alturaAbHuff = 0;
    fread(&alturaAbHuff, sizeof(int), 1, arq);

    bitmap *bm = LeMegaByteDoArquivo(arq);
    
    // transformar isso aqui em "get cabecalho em string"
    char stringFinal[100000] = {0};
    int index = 0; // armazena a posição do bit no bm após a leitura do cabeçalho
    TransformaCabecalhoEmString(stringFinal, bm, &index);

    // transforma a string lida do cabecalho na tabela de codificacao
    char codigo_atual[alturaAbHuff];
    char (*codigos)[alturaAbHuff] = calloc (ASCII_SIZE*alturaAbHuff, sizeof(char));
    if(!codigos) {
        printf("Problema na alocação de memória.\n");
        return EXIT_FAILURE;
    }
    int pos = 0;
    RecontroiCodigos(stringFinal, ASCII_SIZE, alturaAbHuff, codigos, codigo_atual, 0, &pos);

    printf("Tabela de codificação gerada.\n\n");

    // -------------------

    printf("Descompactando...\n");

    DecodificaTexto(arq, bm, index, ASCII_SIZE, alturaAbHuff, codigos, arqtxt);

    printf("Descompactação concluída, arquivo '%s' descompactado com sucesso!\n", argv[1]);
    printf("Arquivo descompactado: %s\n", nomeTxt);

    free(codigos);
    fclose(arqtxt);
    fclose(arq);
    return 0;
}