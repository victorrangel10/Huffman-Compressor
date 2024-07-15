#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
// Definição do tipo de dado arvBin
typedef struct _arvBin arvBin;

// Prototipação das funções
arvBin *CriaArvoreVazia();
int RetornaAlturaArvore(arvBin* ab);
arvBin *CriaArvore(char caracter, int peso, arvBin *right, arvBin *left);
void LiberaArvore(arvBin *arvore);
int EhArvoreVazia(arvBin *arvore);
int PertenceArvore(arvBin *arvore, char c);
void ImprimeArvore(arvBin *a);
char RetornaLetra(arvBin *a);
int RetornaPeso(arvBin * a);
int EhFolha(arvBin * no);
void EscreveCabecalho(bitmap* bm,arvBin * arvore);
void EscreveArvore(bitmap* bm, arvBin *arvore);

// void GeraCodigos(arvBin* raiz, char* codigo_atual, int profundidade, int tam, char codigos[tam][tam]);
void GeraCodigos(arvBin *raiz, char *codigo_atual, int profundidade, int l, int c, char codigos[l][c]);

// void GeraCodigos(arvBin* ab, int l, int c, char tab[l][c], char* caminhoAtual, int nivelAtual);

void LiberaArvore(arvBin *arvore);
#endif // ARVORE_H
