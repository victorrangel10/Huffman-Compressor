#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>

// Definição do tipo de dado node
typedef struct no node;

// Prototipação das funções
node *CriaVazio();
node *CriaArvore(char caracter, int peso, node *right, node *left);
void LiberaArvore(node *arvore);
int EhArvoreVazia(node *arvore);
int PertenceArvore(node *arvore, char c);
void ImprimeArvore(node *a);
char RetornaLetra(node *a);
int RetornaPeso(node * a);
int EhFolha(node * no);
void EscreveArvore(node * arvore);

void GeraCodigos(node* raiz, char* codigo_atual, int profundidade,int tam, char codigos[tam][tam]);


#endif // ARVORE_H
