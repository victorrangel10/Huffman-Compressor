#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Definição do tipo de dado tCell e lista
typedef struct celula tCell;
typedef struct lista lista;


// Prototipação das funções
lista *CriaLista();
int EstaVaziaLista(lista *l);
void InsereLista(lista* l, node* arvore);
node* CriaArvoreHuff(lista* l);
node *RetiraLista(lista *l, node *arvore);
void ImprimeLista(lista *l);
int RetornaTam(lista * l);
node * ObtemPrimeiraArvore(lista *l);
void LiberaListaArvores(lista *l);
#endif // LISTA_H
//
