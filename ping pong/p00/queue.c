#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"


//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila

void queue_append (queue_t **queue, queue_t *elem) {
    queue_t* aux;

    if(queue == NULL){
        printf("ERRO: A fila nao existe\n");
        return;
    }

    if(elem == NULL){
        printf("ERRO: O ele");
        return;
    }

    if(elem->next!=NULL || elem->prev!=NULL){
        printf("ERRO: o elemento pertence a outra fila\n");
        return;
    }


    if((*queue) == NULL){
        (*queue) = elem;
        elem->next = elem;
        elem->prev = elem;
        return;
    }

    aux = *queue;
    if(aux->next == aux && aux->prev == aux){
        aux->next = elem;
        aux->prev = elem;
        elem->next = aux;
        elem->prev = aux;
        return;
    }


    //int i=0;
    while(aux->next != (*queue)){
        aux = aux->next;
        //printf("%d \n", i++);
    }
    aux->next = elem;
    elem->prev = aux;
    elem->next = (*queue);
    (*queue)->prev = elem;

    //printf("a %d -b %d \n", queue, elem);
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

queue_t *queue_remove (queue_t **queue, queue_t *elem) {
    queue_t* aux;
    if(queue == NULL) {
        printf("ERRO: A fila nao existe\n");
        return NULL;
    }
    if((*queue) == NULL){
        printf("ERRO: A fila esta vazia\n");
        return NULL;
    }
    if(elem == NULL){
        printf("O elemento nao existe\n");
        return NULL;
    }

    aux = (*queue);
    if(aux->next == aux && aux->prev == aux){//verifica se a fila tem um elemento
        if((*queue) == elem) {
            printf("a\n");
            *queue == NULL;
            aux->prev = NULL;
            aux->next = NULL;
            return elem;
        }else
            return NULL;
    }else{
        int i=0;
        while(aux->next != (*queue)){
            //printf("\t %d\n", i++);

            if(aux == elem){

                queue_t* ant = aux->prev;
                queue_t* prox = aux->next;

                ant->next = prox;
                prox->prev = ant;
                *queue = prox;

                aux->next = NULL;
                aux->prev = NULL;

                return aux;
            }
            aux=aux->next;
        }
        return NULL;
    }

}

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue){
    queue_t* aux;
    if(queue == NULL)
        return 0;

    aux = queue;
    if(aux->next == aux && aux->prev == aux)
        return 1;
    int i=1;
    while(aux->next != queue){
        aux = aux->next;
        i++;
    }
    return i;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ) {



}
