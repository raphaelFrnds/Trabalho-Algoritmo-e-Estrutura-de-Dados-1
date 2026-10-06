/*
 * lista_encadeada_simples.h
 * Semana 8 - Lista encadeada simples (cada no aponta so para o proximo).
 * Mantem ponteiro para o ultimo no, entao inserir no final e O(1).
 */
#ifndef LISTA_ENCADEADA_SIMPLES_H
#define LISTA_ENCADEADA_SIMPLES_H

#include <stdbool.h>
#include "voo.h"

#ifndef POSICAO_FIM
#define POSICAO_FIM (-1)
#endif

typedef struct NoSimples {
    Voo *voo;
    struct NoSimples *proximo;
} NoSimples;

typedef struct {
    NoSimples *inicio;
    NoSimples *fim;
    int tamanho;
} ListaSimples;

void lista_simples_criar(ListaSimples *l);
bool lista_simples_inserir(ListaSimples *l, Voo *voo, int posicao);
bool lista_simples_remover(ListaSimples *l, int posicao, Voo **removido);
int  lista_simples_buscar(const ListaSimples *l, int id_voo);
/* Voo na posicao dada (0 = inicio), ou NULL se a posicao for invalida. O(n). */
Voo *lista_simples_obter(const ListaSimples *l, int posicao);
int  lista_simples_percorrer(const ListaSimples *l, Voo *destino[], int max);
int  lista_simples_tamanho(const ListaSimples *l);
bool lista_simples_esta_vazia(const ListaSimples *l);
/* Libera os NOS (os voos continuam sendo responsabilidade de quem criou). */
void lista_simples_destruir(ListaSimples *l);

#endif
