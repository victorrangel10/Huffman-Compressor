#include <stdio.h>
#include <string.h>

#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127

void ConstroiArvorePorCodigo(char* string, int size, char codigos[size][size], char codigo_atual[size], int profundidade, int* pos) {
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
        ConstroiArvorePorCodigo(string, size, codigos, codigo_atual, profundidade + 1, pos);
        // madnda pra direita
        codigo_atual[profundidade] = '1';
        ConstroiArvorePorCodigo(string, size, codigos, codigo_atual, profundidade + 1, pos);
    }
}

/* while(nao leu 2 em binario)
{
manda isso aqui pra decodificar na arvore
}
comeca a ler o texto ate o EOF
decodifica o texto lido.
*/

void RecuperaBitMap(FILE* arq, bitmap* bm) {
    /* unsigned char letra;
    int i = 0;
   
    int byte = (unsigned char)fgetc(arq);
    printf("li %x\n", byte);
    while (byte != '\x03') {
        printf("chegou\n");
        append_char(bm, (unsigned char)byte);
        byte = fgetc(arq);
        printf("li %x\n", byte);
    } */
   int byte;

    while ((byte = fgetc(arq)) != EOF && byte != 0x03) {
        printf("li %02X\n", byte); // Debug: imprime o byte lido em hexadecimal
        append_char(bm, (unsigned char)byte);
    }

    if (byte == 0x03) {
        printf("Encontrou o caractere de parada (0x03)\n");
    } else {
        printf("Fim do arquivo ou caractere de parada não encontrado.\n");
    }
}

// TEM QUE INICIALIZAR O BITMAP E LER TUDO

int main() {
    FILE* arq = fopen("coded.bin", "rb");
    bitmap* bm;
    if (!arq) {
        printf("DEU merda");
    }

    char* string = "0001m1s01e1 01b1o";
    char codigos[ASCII_SIZE][ASCII_SIZE], codigo_atual[ASCII_SIZE] = {0};

    for (int i = 0; i < ASCII_SIZE; i++) {
        codigos[i][0] = '\0';
    }

    int pos = 0;
    RecuperaBitMap(arq, bm);
    /* ConstroiArvorePorCodigo(string, ASCII_SIZE, codigos, codigo_atual, 0, &pos);

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (codigos[i][0] != '\0') {
            printf("Caractere %c: %s\n", i, codigos[i]);
        }
    } */

    fclose(arq);
    return 0;
}
