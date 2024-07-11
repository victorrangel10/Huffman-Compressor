#include "arvore.h"
#include <stdio.h>
#include <string.h>

struct no
{
    char caracter;
    int peso;
    node *right_node;
    node *left_node;
};

node *CriaVazio()
{
    return NULL;
}

node *CriaArvore(char caracter, int peso, node *right, node *left)
{
    node *leaf = malloc(sizeof(node));
    leaf->peso = peso;
    leaf->left_node = left;
    leaf->right_node = right;
    leaf->caracter = caracter;
    return leaf;
}

void LiberaArvore(node *arvore)
{
    if (arvore == NULL)
        return;

    LiberaArvore(arvore->right_node);
    LiberaArvore(arvore->left_node);
    free(arvore);
}

int RetornaAlturaArvore(node* ab) {
    if(!ab) return -1; // vazia
    if(!ab->right_node && !ab->left_node) return 1; // folha
    
    int aE = 1 + RetornaAlturaArvore(ab->left_node);
    int aD = 1 + RetornaAlturaArvore(ab->right_node);

    return (aE > aD) ? aE : aD;
}

int EhArvoreVazia(node *arvore)
{
    return arvore == NULL;
}

int PertenceArvore(node *arvore, char c)
{
    if (arvore == NULL)
    {
        return 0;
    }
    if (arvore->caracter == c || PertenceArvore(arvore->left_node, c) || PertenceArvore(arvore->right_node, c))
    {
        return 1;
    }
    else
        return 0;
}

int RetornaPeso(node *a)
{
    return a->peso;
}
char RetornaLetra(node *a)
{
    return a->caracter;
}

int EhFolha(node *no)
{
    return (EhArvoreVazia(no->left_node) && EhArvoreVazia(no->right_node));
}

void EscreveArvore(bitmap *bm, node *arvore)
{
    if (EhFolha(arvore))
    {
        printf("1%c", RetornaLetra(arvore));
        bitmapAppendLeastSignificantBit(bm, (unsigned char)'1');
        bitmap_append_char(bm, (unsigned char)RetornaLetra(arvore));
    }
    else
    {
        printf("0");
        bitmapAppendLeastSignificantBit(bm, (unsigned char)'0');
        EscreveArvore(bm, arvore->left_node);
        EscreveArvore(bm, arvore->right_node);
    }
}

void EscreveCabecalho(bitmap *bm, node *arvore)
{
    printf("\nArvore escrita em binario: \n");
    EscreveArvore(bm, arvore);
    /* coloca dois bits '1' para indicar que o cabecalho acabou*/
    for (size_t i = 0; i < 2; i++)
    {
        bitmapAppendLeastSignificantBit(bm, '1');
    }
    printf("\n");
}

void ImprimeArvore(node *a)
{
    if (EhFolha(a))
    {
        printf("%c", RetornaLetra(a));
    }
    else
    {
        printf("<0 ");
        ImprimeArvore(a->left_node);
        ImprimeArvore(a->right_node);
        printf(" >");
    }
}

// void GeraCodigos(node* ab, int l, int c, char tab[l][c], char* caminhoAtual, int nivelAtual) {
//     if(!ab) return;
    
//     if(!ab->right_node && !ab->left_node) { // folha: possui um caracter
//         strcpy(tab[(int)ab->caracter], caminhoAtual);
//     } else {
//         char caminhoEsquerda[nivelAtual + 1];
//         char caminhoDireita[nivelAtual + 1];

//         if(nivelAtual == 1) {
//             sprintf(caminhoEsquerda, "0");
//             sprintf(caminhoDireita, "1");
//         } else {
//             sprintf(caminhoEsquerda, "%s0",caminhoAtual);
//             sprintf(caminhoDireita, "%s1",caminhoAtual);
//         }

//         nivelAtual++;
//         GeraCodigos(ab->right_node, l, c, tab, caminhoDireita, nivelAtual);
//         GeraCodigos(ab->left_node, l, c, tab, caminhoEsquerda, nivelAtual);
//     }
// }

void GeraCodigos(node *raiz, char *codigo_atual, int profundidade, int tam, char codigos[tam][tam])
{
    if (raiz == NULL)
        return;

    // Verifica se é um nó folha
    if (raiz->left_node == NULL && raiz->right_node == NULL)
    {
        codigo_atual[profundidade] = '\0'; // Termina a string do código
        strcpy(codigos[(unsigned char)raiz->caracter], codigo_atual);
    }
    // Adiciona '0' para a subárvore esquerda
    if (raiz->left_node != NULL)
    {
        codigo_atual[profundidade] = '0';
        GeraCodigos(raiz->left_node, codigo_atual, profundidade + 1, tam, codigos);
    }
    // Adiciona '1' para a subárvore direita
    if (raiz->right_node != NULL)
    {
        codigo_atual[profundidade] = '1';
        GeraCodigos(raiz->right_node, codigo_atual, profundidade + 1, tam, codigos);
    }
}
