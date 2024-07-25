#include "listaArvores.h"
#include "arvore.h"
#include <stdio.h>

typedef struct celula tCell;

struct celula{
	arvBin * arvore;
	tCell * proxima;
};

struct lista{
	tCell * prim;
	tCell * ult;
	int tam;
};

lista * CriaLista(){
	lista * l = malloc(sizeof(lista));
	l->ult = l->prim = NULL;
	l->tam =0;
	return l;
}

int EstaVaziaLista(lista * l){
	return l->prim==NULL;
}

void InsereLista(lista* l, arvBin* arvore){
	l->tam++;
	tCell* celula = malloc(sizeof(tCell));
	celula->arvore = arvore;
	celula->proxima = NULL;
	
	// caso lista vazia
	if (l->prim == NULL) {
		l->prim=l->ult=celula;
		return;
	}
	// se nao for vazia
	tCell* celAtual = l->prim;
	tCell* celAnterior = NULL;

	while (celAtual)
	{
		if(RetornaPeso(celAtual->arvore) >= RetornaPeso(arvore)) {
			if(celAnterior == NULL) {
				celula->proxima = l->prim;
				l->prim = celula;
			} else {
				celAnterior->proxima = celula;
				celula->proxima = celAtual;
			}
			return;
		}
		celAnterior = celAtual;
		celAtual = celAtual->proxima;
	}
	l->ult->proxima = celula;
	l->ult = celula;
}

arvBin* CriaArvoreHuff(lista* l) {
	if(EstaVaziaLista(l)) return NULL; // lista vazia;
	if(l->prim == l->ult) { // apenas 1 elemento
		arvBin* ab = l->prim->arvore;
		free(l->prim);
		free(l);
		return ab;
	}

	int peso = RetornaPeso(l->prim->arvore) + RetornaPeso(l->prim->proxima->arvore);
	arvBin* ab = CriaArvore(0, peso, l->prim->proxima->arvore, l->prim->arvore);

	tCell* aux = l->prim;
	tCell* aux2 = l->prim->proxima;
	l->tam -= 2;
	l->prim = l->prim->proxima->proxima;
	free(aux);
	free(aux2);

	InsereLista(l, ab);
	return CriaArvoreHuff(l);
}

arvBin* RetiraLista(lista * l, arvBin * arvore){
	tCell * anterior = NULL;
	tCell * celulaAtual = l->prim;
	while (celulaAtual != NULL && celulaAtual->arvore!=arvore) {
		anterior = celulaAtual;
		celulaAtual=celulaAtual->proxima;	
	}
	if(celulaAtual == NULL){
		return NULL;
	}
	l->tam--;
	//caso unico item
	if (l->prim ==celulaAtual && celulaAtual == l->ult) {
		l->prim = l->ult = NULL;
		arvBin * a = celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}


	//caso primeiro item
	if (l->prim->arvore == arvore) {
		arvBin* a =l->prim->arvore;
		l->prim = l->prim->proxima;
		free(celulaAtual);
		return a;
	}

	//caso ultimo item
	if (l->ult->arvore == arvore) {
		l->ult = anterior;
		anterior->proxima = NULL;
		arvBin * a =celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}

	else {
		anterior->proxima = celulaAtual->proxima;
		arvBin * a =celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}
}

void ImprimeLista(lista* l){
	tCell* celula = l->prim;
	while (celula!=NULL) {
		printf("//// LISTA //////\n");
		ImprimeArvore(celula->arvore); 
		//printf("letra %c peso %d\n",RetornaLetra(celula->arvore),RetornaPeso(celula->arvore));
		celula = celula->proxima;
	}	
}

int RetornaTam(lista * l){
	return l->tam;
}

arvBin* ObtemPrimeiraArvore(lista *l){
	return l->prim->arvore;
}

void LiberaListaArvores(lista *l){
	tCell *atual=l->prim;
    tCell *prox=NULL;

    while (atual != NULL)
    {
        prox = atual->proxima;
		LiberaArvore(atual->arvore);
        free(atual);
        atual = prox;
    }
	
    free(l);
}