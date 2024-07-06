#include "listaArvores.h"
#include "arvore.h"
#include <stdio.h>


struct celula{
	node * arvore;
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

void InsereFinalLista(lista* l, node * arvore){
	tCell* celula = malloc(sizeof(tCell));
	celula->arvore = arvore;
	// caso lista vazia
	if (l->prim == NULL && l->ult == NULL) { 
		l->prim=l->ult=celula;
		l->prim->proxima = NULL;
		l->tam++;
		return;
	}
	// se nao for vazia
	l->ult->proxima = celula;
	l->ult = celula;
	celula->proxima = NULL;
	l->tam++;
}


node* RetiraLista(lista * l, node * arvore){
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
		node * a = celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}


	//caso primeiro item
	if (l->prim->arvore == arvore) {
		node* a =l->prim->arvore;
		l->prim = l->prim->proxima;
		free(celulaAtual);
		return a;
	}

	//caso ultimo item
	if (l->ult->arvore == arvore) {
		l->ult = anterior;
		anterior->proxima = NULL;
		node * a =celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}

	else {
		anterior->proxima = celulaAtual->proxima;
		node * a =celulaAtual->arvore;
		free(celulaAtual);
		return a;
	}
}

void OrdenaLista(lista* l){
	if(!l) return;
	tCell * aux, *atual, *proxima;
	atual = l->prim;
	while (atual!=NULL) {
	proxima = atual->proxima;		
		while (proxima!=NULL) {
			if (RetornaPeso(proxima->arvore)<RetornaPeso(atual->arvore)) {
			node * aux;
			aux = atual->arvore;
			atual->arvore = proxima->arvore;
			proxima->arvore = aux;
			}
			proxima = proxima->proxima;
		}
		atual = atual->proxima;
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


node * SomaDuasPrimeirasArvores(lista *l, node **t1, node **t2){
	*t1 = l->prim->arvore;
	*t2= l->prim->proxima->arvore;
	node *tr = CriaArvore('\0', RetornaPeso(*t1) + RetornaPeso(*t2), *t1, *t2);
	return tr;
}


node * ObtemPrimeiraArvore(lista *l){
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