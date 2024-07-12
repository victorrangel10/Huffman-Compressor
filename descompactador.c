#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127
#define MEGABYTE 8388608
// #define BM_TAM MEGABYTE-512
// #define BM_TAM 8388608

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
    /* como i pode ser incrementado nos valores 1 e 8, o "falsoI"
    vai ser incrementado apenas em 1 para contar as iteracoes*/

    int flag = 0, i = 0, falsoI = 0;
    unsigned char bitAtual = '\0';

    while (1) {
        bitAtual = bitmapGetBit(bm, i) + '0';
        //  printf("LI o bit %c\n",bitmapGetBit(bm, i) + '0');
        falsoI++;

        if (bitAtual == '0') {
            //  printf("entrou no 0\n");
            string[falsoI - 1] = '0'; // representação de nó não folha
            i++;
            continue;
        }
        else if (bitAtual == '1') { // nó folha ou possível condição de parada

            if ((bitmapGetBit(bm, i + 1) + '0') == '1') {
            // condição de parada: encontrar a sequência '11' (indica o fim da árvore escrita no cabeçalho)
                string[falsoI] = '\0';
                // printf("%s\n", string);
                *index = i;
                // printf("chegou no final do cabecalho\n");
                break;
            }

            string[falsoI - 1] = '1'; // representação do nó folha

            // leitura do caracter contido no nó folha
            unsigned char byte = 0x00;
            int j;
            for (j = 0; j < 8; j++) {
                unsigned char bit = bitmapGetBit(bm, i + 1 + j);
                //  printf("Mandando o bit %c para construir a letra\n",bitmapGetBit(bm, i+1+j) + '0');
                byte |= (bit << (7 - j));
                // printf("byte eh %02X\n",byte);
            }
            
            falsoI++;
            string[falsoI - 1] = byte;

            // printf("ENVIANDO A LETRA %b\n",byte);
            // printf("ENVIANDO A LETRA NA STRING %c\n",string[falsoi-1]);
            i += j + 1;
        }
    }
}

bitmap *LeMegaByteDoArquivo(FILE *arqbin) {
    unsigned char byte;
    // cria bitmap com tamanho BM_TAM
    bitmap *bm = bitmapInit((unsigned int)MEGABYTE);

    int flagFinaliza = 0;

    /*pega byte a byte do arquivo comprimido ate o tamanho do bitmap ser igual a BM_TAM
     ou seja, encher o bitmap */

    while (!flagFinaliza && fread(&byte, sizeof(unsigned char), 1, arqbin) == 1) {
        //escreve o byte bit a bit
        // if(bitmapGetLength(bm) + 8 >= BM_TAM) break;
        for (int i = 7; i >= 0; i--) {
            //printf("colocando bit %d\n",counter);
            if (bitmapGetLength(bm) < MEGABYTE)
                bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);

            else {
                printf("acabou o espaco no bm\n");
                flagFinaliza = 1;
                break;
            }
        }
    }
    return bm;
}

// void LeBitsEAnexa(bitmap* bm, int qtdBits, FILE* arqbin) {
//     unsigned char byte;

//     int b = 0, finaliza = 0;
//     // int qtdBytes = (qtdBits + 7) / 8;

//     /*pega byte a byte do arquivo comprimido ate o tamanho do bitmap ser igual a BM_TAM
//      ou seja, encher o bitmap */
//     while (fread(&byte, sizeof(unsigned char), 1, arqbin) == 1 && !finaliza) {
//         //escreve o byte bit a bit
//         for (int i = 7; i >= 0; i--) {
//             b++;
//             //printf("colocando bit %d\n",counter);
//             if(b < bitmapGetLength(bm))
//                 bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);
//             else
//                 finaliza = 1;
//                 break;
//         }
//         b++;
//     }
// }

void DecodificaTexto(FILE* arqbin, bitmap *bm, int index, int l, int c, char codigos[l][c],FILE * arqsaida) {
    char codigo[c];
    int i = 0, j = 0, found_end_char = 0;

    while (!found_end_char) {
       if (index == MEGABYTE) {
            fprintf(arqsaida,"|-|");

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
                    fputc(i,arqsaida);
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

    int alturaAbHuff = 0;
    fread(&alturaAbHuff, sizeof(int), 1, arq);
    printf("altura: %d\n", alturaAbHuff);

    char codigo_atual[alturaAbHuff];
    char (*codigos)[alturaAbHuff] = calloc (ASCII_SIZE*alturaAbHuff, sizeof(char));

    bitmap *bm = LeMegaByteDoArquivo(arq);

    // transformar isso aqui em "get cabecalho em string"
    char stringFinal[100000] = {0};
    int index = 0;
    TransformaCabecalhoEmString(stringFinal, bm, &index);


    // transforma a string lida do cabecalho na tabela de codificacao
    int pos = 0;
    RecontroiCodigos(stringFinal, ASCII_SIZE, alturaAbHuff, codigos, codigo_atual, 0, &pos);

    DecodificaTexto(arq,bm, index, ASCII_SIZE, alturaAbHuff, codigos, arqtxt);

    free(codigos);
    fclose(arqtxt);
    fclose(arq);
    return 0;
}
