#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
// Definição do tipo de dado arvBin
typedef struct _arvBin arvBin;

// Prototipação das funções

/**
 * @brief Cria uma árvore vazia.
 * 
 * @return Ponteiro para uma árvore binária vazia.
 */
arvBin *CriaArvoreVazia();

/**
 * @brief Cria uma árvore com um nó raiz.
 * 
 * @param caracter Caractere armazenado no nó raiz.
 * @param peso Peso do nó raiz.
 * @param right Ponteiro para a subárvore direita.
 * @param left Ponteiro para a subárvore esquerda.
 * @return Ponteiro para a árvore criada.
 */
arvBin *CriaArvore(char caracter, int peso, arvBin *right, arvBin *left);

/**
 * @brief Libera a memória alocada para a árvore.
 * 
 * @param arvore Ponteiro para a árvore a ser liberada.
 */
void LiberaArvore(arvBin *arvore);

/**
 * @brief Retorna a altura da árvore.
 * 
 * @param ab Ponteiro para a árvore.
 * @return Altura da árvore.
 */
int RetornaAlturaArvore(arvBin* ab);

/**
 * @brief Verifica se a árvore está vazia.
 * 
 * @param arvore Ponteiro para a árvore.
 * @return 1 se a árvore estiver vazia, 0 caso contrário.
 */
int EhArvoreVazia(arvBin *arvore);

/**
 * @brief Verifica se um caractere pertence à árvore.
 * 
 * @param arvore Ponteiro para a árvore.
 * @param c Caractere a ser verificado.
 * @return 1 se o caractere pertence à árvore, 0 caso contrário.
 */
int PertenceArvore(arvBin *arvore, char c);

/**
 * @brief Retorna o peso de um nó da árvore.
 * 
 * @param a Ponteiro para o nó da árvore.
 * @return Peso do nó.
 */
int RetornaPeso(arvBin *a);

/**
 * @brief Retorna o caractere armazenado em um nó da árvore.
 * 
 * @param a Ponteiro para o nó da árvore.
 * @return Caractere armazenado no nó.
 */
char RetornaLetra(arvBin *a);

/**
 * @brief Verifica se um nó é uma folha.
 * 
 * @param no Ponteiro para o nó da árvore.
 * @return 1 se o nó é uma folha, 0 caso contrário.
 */
int EhFolha(arvBin *no);

/**
 * @brief Escreve a árvore em um bitmap.
 * 
 * @param bm Ponteiro para o bitmap.
 * @param arvore Ponteiro para a árvore.
 */
void EscreveArvore(bitmap *bm, arvBin *arvore);

/**
 * @brief Escreve o cabeçalho da árvore em um bitmap.
 * 
 * @param bm Ponteiro para o bitmap.
 * @param arvore Ponteiro para a árvore.
 */
void EscreveCabecalho(bitmap *bm, arvBin *arvore);

/**
 * @brief Imprime a árvore na saída padrão.
 * 
 * @param a Ponteiro para a árvore.
 */
void ImprimeArvore(arvBin *a);

/**
 * @brief Gera códigos para os caracteres armazenados na árvore.
 * 
 * @param raiz Ponteiro para a raiz da árvore.
 * @param codigo_atual String que representa o código atual.
 * @param profundidade Profundidade atual na árvore.
 * @param l Número de linhas da matriz de códigos.
 * @param c Número de colunas da matriz de códigos.
 * @param codigos Matriz que armazenará os códigos.
 */
void GeraCodigos(arvBin *raiz, char *codigo_atual, int profundidade, int l, int c, char codigos[l][c]);

#endif // ARVORE_H

