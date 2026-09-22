/*
 * pilha_array.h
 * Semana 10 - Pilha (LIFO) com vetor de tamanho fixo.
 * Guarda niveis de Prioridade: e o que o desfazer/refazer da
 * Semana 11 precisa empilhar.
 */
#ifndef PILHA_ARRAY_H
#define PILHA_ARRAY_H

#include <stdbool.h>
#include "voo.h"

#define PILHA_CAP 50

typedef struct {
    Prioridade itens[PILHA_CAP];
    int topo;                 /* quantidade de itens; o topo e itens[topo-1] */
} PilhaArray;

void pilha_array_criar(PilhaArray *p);
bool pilha_array_empilhar(PilhaArray *p, Prioridade item);      /* false se cheia */
bool pilha_array_desempilhar(PilhaArray *p, Prioridade *saida); /* false se vazia */
bool pilha_array_topo(const PilhaArray *p, Prioridade *saida);  /* false se vazia */
bool pilha_array_esta_vazia(const PilhaArray *p);
bool pilha_array_esta_cheia(const PilhaArray *p);
int  pilha_array_tamanho(const PilhaArray *p);

#endif
