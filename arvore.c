#include "arvore.h"
#include <stdio.h>
#include <string.h>

struct _arvBin
{
    char caracter;
    int peso;
    arvBin *right_arvBin;
    arvBin *left_arvBin;
};

arvBin *CriaArvoreVazia()
{
    return NULL;
}

arvBin *CriaArvore(char caracter, int peso, arvBin *right, arvBin *left)
{
    arvBin *leaf = malloc(sizeof(arvBin));
    leaf->peso = peso;
    leaf->left_arvBin = left;
    leaf->right_arvBin = right;
    leaf->caracter = caracter;
    return leaf;
}

void LiberaArvore(arvBin *arvore)
{
    if (arvore == NULL)
        return;

    LiberaArvore(arvore->right_arvBin);
    LiberaArvore(arvore->left_arvBin);
    free(arvore);
}

int RetornaAlturaArvore(arvBin* ab) {
    if(!ab) return -1; // vazia
    if(!ab->right_arvBin && !ab->left_arvBin) return 1; // folha
    
    int aE = 1 + RetornaAlturaArvore(ab->left_arvBin);
    int aD = 1 + RetornaAlturaArvore(ab->right_arvBin);

    return (aE > aD) ? aE : aD;
}

int EhArvoreVazia(arvBin *arvore)
{
    return arvore == NULL;
}

int PertenceArvore(arvBin *arvore, char c)
{
    if (arvore == NULL)
    {
        return 0;
    }
    if (arvore->caracter == c || PertenceArvore(arvore->left_arvBin, c) || PertenceArvore(arvore->right_arvBin, c))
    {
        return 1;
    }
    else
        return 0;
}

int RetornaPeso(arvBin *a)
{
    return a->peso;
}

char RetornaLetra(arvBin *a)
{
    return a->caracter;
}

int EhFolha(arvBin *no)
{
    return (EhArvoreVazia(no->left_arvBin) && EhArvoreVazia(no->right_arvBin));
}

void EscreveArvore(bitmap *bm, arvBin *arvore)
{
    if (EhFolha(arvore))
    {
        // printf("1%c", RetornaLetra(arvore));
        bitmapAppendLeastSignificantBit(bm, (unsigned char)'1');
        bitmap_append_char(bm, (unsigned char)RetornaLetra(arvore));
    }
    else
    {
        // printf("0");
        bitmapAppendLeastSignificantBit(bm, (unsigned char)'0');
        EscreveArvore(bm, arvore->left_arvBin);
        EscreveArvore(bm, arvore->right_arvBin);
    }
}

void EscreveCabecalho(bitmap *bm, arvBin *arvore) {
    EscreveArvore(bm, arvore);

    /* coloca dois bits '1' para indicar que o cabecalho acabou*/
    // motivo: nenhum dos primeiros 128 caracteres ASCII começam com '11' em sua codificação binária
    bitmapAppendLeastSignificantBit(bm, '1');
    bitmapAppendLeastSignificantBit(bm, '1');
}

void ImprimeArvore(arvBin *a)
{
    if (EhFolha(a))
        printf("%c", RetornaLetra(a));
    else {
        printf("<0 ");
        ImprimeArvore(a->left_arvBin);
        ImprimeArvore(a->right_arvBin);
        printf(" >");
    }
}

void GeraCodigos(arvBin *raiz, char *codigo_atual, int profundidade, int l, int c, char codigos[l][c])
{
    if (raiz == NULL)
        return;

    // Verifica se é um nó folha
    if (raiz->left_arvBin == NULL && raiz->right_arvBin == NULL)
    {
        codigo_atual[profundidade] = '\0'; // Termina a string do código
        strcpy(codigos[(unsigned char)raiz->caracter], codigo_atual);
    }
    // Adiciona '0' para a subárvore esquerda
    if (raiz->left_arvBin != NULL)
    {
        codigo_atual[profundidade] = '0';
        GeraCodigos(raiz->left_arvBin, codigo_atual, profundidade + 1, l, c, codigos);
    }
    // Adiciona '1' para a subárvore direita
    if (raiz->right_arvBin != NULL)
    {
        codigo_atual[profundidade] = '1';
        GeraCodigos(raiz->right_arvBin, codigo_atual, profundidade + 1, l, c, codigos);
    }
}
