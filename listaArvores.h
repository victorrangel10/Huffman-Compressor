#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Definição do tipo de dado tCell e lista
typedef struct celula tCell;
typedef struct lista lista;


// Prototipação das funções
/**
 * @brief Cria uma lista vazia.
 * 
 * @return Ponteiro para a lista criada.
 */
lista *CriaLista();

/**
 * @brief Verifica se a lista está vazia.
 * 
 * @param l Ponteiro para a lista.
 * @return 1 se a lista estiver vazia, 0 caso contrário.
 */
int EstaVaziaLista(lista *l);

/**
 * @brief Insere uma árvore na lista de forma ordenada pelo peso.
 * 
 * @param l Ponteiro para a lista.
 * @param arvore Ponteiro para a árvore a ser inserida.
 */
void InsereLista(lista *l, arvBin *arvore);

/**
 * @brief Cria a árvore de Huffman a partir da lista de árvores.
 * 
 * @param l Ponteiro para a lista.
 * @return Ponteiro para a árvore de Huffman criada.
 */
arvBin *CriaArvoreHuff(lista *l);

/**
 * @brief Remove uma árvore específica da lista.
 * 
 * @param l Ponteiro para a lista.
 * @param arvore Ponteiro para a árvore a ser removida.
 * @return Ponteiro para a árvore removida, ou NULL se não encontrada.
 */
arvBin *RetiraLista(lista *l, arvBin *arvore);

/**
 * @brief Imprime as árvores contidas na lista.
 * 
 * @param l Ponteiro para a lista.
 */
void ImprimeLista(lista *l);

/**
 * @brief Retorna o tamanho da lista.
 * 
 * @param l Ponteiro para a lista.
 * @return Tamanho da lista.
 */
int RetornaTam(lista *l);

/**
 * @brief Obtém a primeira árvore da lista.
 * 
 * @param l Ponteiro para a lista.
 * @return Ponteiro para a primeira árvore da lista.
 */
arvBin *ObtemPrimeiraArvore(lista *l);

/**
 * @brief Libera a memória alocada para a lista e suas árvores.
 * 
 * @param l Ponteiro para a lista a ser liberada.
 */
void LiberaListaArvores(lista *l);
#endif // LISTA_H
//
