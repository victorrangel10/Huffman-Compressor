#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127
#define MEGABYTE 8388608
#define BM_TAM MEGABYTE-100

void reconstroi_tabela(char *string, int size, char codigos[size][size], char codigo_atual[size], int profundidade, int *pos)
{
    if (string[*pos] == '\0')
    {
        return;
    }

    if (string[*pos] == '1')
    {
        (*pos)++;
        codigo_atual[profundidade] = '\0';
        char caracter = string[(*pos)];
        strcpy(codigos[(unsigned char)caracter], codigo_atual);
        (*pos)++;
    }
    else if (string[*pos] == '0')
    {
        (*pos)++;
        // manda pra esquerda
        codigo_atual[profundidade] = '0';
        reconstroi_tabela(string, size, codigos, codigo_atual, profundidade + 1, pos);
        // madnda pra direita
        codigo_atual[profundidade] = '1';
        reconstroi_tabela(string, size, codigos, codigo_atual, profundidade + 1, pos);
    }
}

int eh_igual_codigo(char *codigo, char *string)
{
    if (strcmp(codigo, string) == 0)
    {
        return 1;
    }
    else
        return 0;
}

void transforma_cabecalho_em_string(char *string, bitmap *bm, int *index)
{
    /* como i pode ser incrementado nos valores 1 e 8, o "falsoi"
    vai ser incrementado apenas em 1 para contar as iteracoes*/

    int flag = 0, i = 0, falsoi = 0;
    unsigned char bitAtual = '\0';

    while (!flag)
    {
        bitAtual = bitmapGetBit(bm, i) + '0';
        //  printf("LI o bit %c\n",bitmapGetBit(bm, i) + '0');
        falsoi++;

        if (bitAtual == '0')
        {
            //  printf("entrou no 0\n");
            string[falsoi - 1] = '0';
            i++;
            continue;
        }
        else if (bitAtual == '1')
        {
            // nenhum char comeca com 1, por isso, a sequencia 11 se torna impossivel, sendo utilizada para indicar o fim do cabecalho
            if ((bitmapGetBit(bm, i + 1) + '0') == '1')
            {
                flag = 1;
                i += 2;
                // printf("chegou no final do cabecalho\n");
                break;
            }

            unsigned char byte = 0x00;
            int j;
            for (j = 0; j < 8; j++)
            {
                unsigned char bit = bitmapGetBit(bm, i + 1 + j);
                //  printf("Mandando o bit %c para construir a letra\n",bitmapGetBit(bm, i+1+j) + '0');
                byte |= (bit << (7 - j));
                // printf("byte eh %02X\n",byte);
            }
            string[falsoi - 1] = '1';
            falsoi++;
            string[falsoi - 1] = byte;

            // printf("ENVIANDO A LETRA %b\n",byte);
            // printf("ENVIANDO A LETRA NA STRING %c\n",string[falsoi-1]);
            i += j + 1;
        }
    }

    string[falsoi] = '\0';
    printf("%s\n", string);
    *index = i;
}



bitmap *LeMegaByteDoArquivo(FILE *arqbin)
{
    unsigned char byte;
    // cria bitmap com tamanho BM_TAM
    bitmap *bm = bitmapInit((unsigned int)BM_TAM);

    int counter=0,flag=0;

    /*pega byte a byte do arquivo comprimido ate o tamanho do bitmap ser igual a BM_TAM
     ou seja, encher o bitmap */

    while (fread(&byte, sizeof(unsigned char), 1, arqbin) == 1 && !flag)
    {
        //escreve o byte bit a bit
        for (int i = 7; i >= 0; i--)
        {
            //printf("colocando bit %d\n",counter);
            if (bitmapGetLength(bm) < BM_TAM)
            {
                 bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);
            }else{
                printf("acabou o espaco no bm\n");
                flag=1;
                break;
            }
            counter++;
        }
    
    }
    return bm;
}

void decodifica_textos(FILE* arqbin,bitmap *bm, int index, int tam, char codigos[tam][tam],FILE * arqsaida)
{
    char codigo[1000];
    int i = 0, j = 0, found_end_char=0;

    while (!found_end_char)
    {
       


        codigo[j] = bitmapGetBit(bm, index) + '0';
        codigo[j + 1] = '\0';

        j++;
        index++;

        for (size_t i = 0; i < tam; i++)
        {
            if (eh_igual_codigo(codigo, codigos[i]))
            {
                if(i==3){
                    printf("CHEGOU NO FINAL\n");
                    found_end_char=1;
                    break;
                }
                fputc(i,arqsaida);
              //  printf("%c", i);
                j = 0;
            }
        }
        if (index == BM_TAM) {
            fprintf(arqsaida," -- vai trocar bm agora --");
            bitmapLibera(bm);
            bm = LeMegaByteDoArquivo(arqbin);
            index = 0;  
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *arq = fopen("arquivos/compressao.comp", "rb");
    char stringFinal[100000];
    if (!arq)
    {
        printf("Nao conseguiu abrir o arquivon\n");
    }

    FILE *arqtxt = fopen("arquivos/saida.txt", "w");
    if (arqtxt == NULL)
    {
        perror("Erro ao abrir arquivo txt");
        return EXIT_FAILURE;
    }

    char codigos[ASCII_SIZE][ASCII_SIZE], codigo_atual[ASCII_SIZE] = {0};
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        codigos[i][0] = '\0';
    }

    int pos = 0;
    
    bitmap *bm = LeMegaByteDoArquivo(arq);
   
    int index = 0;

    // a partir do bitmap lido, transforma conteudo em string capaz de gerar arvore ou tabela de codificacao
    /*  `index` eh o indice onde comeca o conteudo do arquivo, imediatamente apos o cabecalho*/

    // transformar isso aqui em "get cabecalho em string"
    transforma_cabecalho_em_string(stringFinal, bm, &index);

    // printf(" index eh %d\n",index);

    // transforma a string lida do cabecalho na tabela de codificacao
    reconstroi_tabela(stringFinal, ASCII_SIZE, codigos, codigo_atual, 0, &pos);

    for (int i = 0; i < ASCII_SIZE; i++)
    {
        if (codigos[i][0] != '\0')
        {
            printf("Caractere %c (%d): %s\n", i,i, codigos[i]);
        }
    }

    decodifica_textos(arq,bm, index, ASCII_SIZE, codigos, arqtxt);

    bitmapLibera(bm);
    fclose(arqtxt);
    fclose(arq);
    return 0;
}
