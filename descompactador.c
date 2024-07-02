#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127

void reconstroi_tabela(char* string, int size, char codigos[size][size], char codigo_atual[size], int profundidade, int* pos) {
    if (string[*pos] == '\0') {
        return;
    }

    if (string[*pos] == '1') {
        (*pos)++;
        codigo_atual[profundidade] = '\0';
        char caracter = string[(*pos)];
        strcpy(codigos[(unsigned char)caracter], codigo_atual);
        (*pos)++;
    } else if (string[*pos] == '0') {
        (*pos)++;
        // manda pra esquerda
        codigo_atual[profundidade] = '0';
        reconstroi_tabela(string, size, codigos, codigo_atual, profundidade + 1, pos);
        // madnda pra direita
        codigo_atual[profundidade] = '1';
        reconstroi_tabela(string, size, codigos, codigo_atual, profundidade + 1, pos);
    }
}

bitmap* LeituraBitmapDoArquivo(FILE* arqbin, int tamanhoCabecalho) {
    unsigned char byte;
    bitmap* bm = bitmapInit(1024);
    while (fread(&byte, sizeof(unsigned char), 1, arqbin) == 1) {
        for (int i = 7; i >= 0; i--) {
            bitmapAppendLeastSignificantBit(bm, (byte >> i) & 1);
        }
    }
    return bm;
}

void transforma_bitmap_em_string(char* string, bitmap* bm, int* index) {
    /* como i pode ser incrementado nos valores 1 e 8, o "falsoi"
    vai ser incrementado apenas em 1 para contar as iteracoes*/

    int flag = 0, i = 0, falsoi = 0;
    unsigned char bitAtual = '\0';

    while (!flag) {
        bitAtual = bitmapGetBit(bm, i) + '0';
        //  printf("LI o bit %c\n",bitmapGetBit(bm, i) + '0');
        falsoi++;

        if (bitAtual == '0') {
            //  printf("entrou no 0\n");
            string[falsoi - 1] = '0';
            i++;
            continue;
        } else if (bitAtual == '1') {
            // nenhum char comeca com 1, por isso, a sequencia 11 se torna impossivel, sendo utilizada para indicar o fim do cabecalho
            if ((bitmapGetBit(bm, i + 1) + '0') == '1') {
                flag = 1;
                i += 2;
                // printf("chegou no final do cabecalho\n");
                break;
            }

            unsigned char byte = 0x00;
            int j;
            for (j = 0; j < 8; j++) {
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
int eh_igual_codigo(char * codigo, char * string){
    if (strcmp(codigo,string)==0)
    {
        return 1;
    }
    else return 0;
}


void decodifica_textos(bitmap* bm, int index, int tam, char codigos[tam][tam], int max_size) {
    char codigo[max_size];
    int i = 0,j=0;
    /* codigo[0] = bitmapGetBit(bm, index) + '0';
    printf("primeiro bit lido %c\n", codigo[0]);
    codigo[1] = bitmapGetBit(bm, index + 1) + '0';
    codigo[2] = bitmapGetBit(bm, index + 2) + '0';
    codigo[3] = bitmapGetBit(bm, index + 3) + '0';
    codigo[4] = '\0';
    printf("codigo buscado eh: %s\n", codigo); */

    while (index<bitmapGetLength(bm)/8)
    {
      /* printf(" index eh: %d\n",index);
      printf(" length eh: %d\n",bitmapGetLength(bm));
      printf("Adicionando %c ao codigo\n",bitmapGetBit(bm,index)+'0'); */

      codigo[j] = bitmapGetBit(bm,index)+'0';

      codigo[j+1] = '\0';

      j++;
      index++;
      /* printf("TAM EH %d\n",tam);
      printf(" buscando por: %s\n",codigo); */

      for (size_t i = 0; i < tam; i++)
      {
        if (eh_igual_codigo(codigo, codigos[i]))
        {
            printf(" ACHOU LETRA %c\n",i);
            j=0;
        } 
      }
      /* printf("SAiu uma vez"); */
    }
    
}

int main() {
    FILE* arq = fopen("coded.bin", "rb");
    char stringFinal[100];
    if (!arq) {
        printf("Nao conseguiu abrir o arquivon\n");
    }

    char codigos[ASCII_SIZE][ASCII_SIZE], codigo_atual[ASCII_SIZE] = {0};
    for (int i = 0; i < ASCII_SIZE; i++) {
        codigos[i][0] = '\0';
    }

    int pos = 0;
    bitmap* bm = LeituraBitmapDoArquivo(arq, 30);
    int index = 0;

    // a partir do bitmap lido, transforma conteudo em string capaz de gerar arvore ou tabela de codificacao
    /*  `index` eh o indice onde comeca o conteudo do arquivo, imediatamente apos o cabecalho*/
    transforma_bitmap_em_string(stringFinal, bm, &index);

    // transforma a string lida na tabela de codificacao
    reconstroi_tabela(stringFinal, ASCII_SIZE, codigos, codigo_atual, 0, &pos);

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (codigos[i][0] != '\0') {
            printf("Caractere %c: %s\n", i, codigos[i]);
        }
    }
    printf("bit no index eh %c\n",bitmapGetBit(bm,index)+'0');
    decodifica_textos(bm, index, ASCII_SIZE, codigos, 1000);
    bitmapLibera(bm);
    fclose(arq);
    return 0;
}
