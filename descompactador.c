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

int main() {
    FILE* arq = fopen("teste.bin", "rb");
    char* string = "0001m1s01e1 01b1o";
    char codigos[ASCII_SIZE][ASCII_SIZE], codigo_atual[ASCII_SIZE];
    int pos =0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        codigos[i][0] = '\0';
    }
    ConstroiArvorePorCodigo(string, ASCII_SIZE, codigos, codigo_atual, 0,&pos);

       for (int i = 0; i < ASCII_SIZE; i++) {
        if (codigos[i][0] != '\0') {
            printf("Caractere %c: %s\n", i, codigos[i]);
        }
    }


    return 0;
}
