/*
 * pilha_encadeada.h
 * Semana 10 - Pilha (LIFO) com nos encadeados (sem limite de tamanho).
 */
#ifndef PILHA_ENCADEADA_H
#define PILHA_ENCADEADA_H

#include <stdbool.h>
#include "voo.h"

typedef struct NoPilha {
    Prioridade item;
    struct NoPilha *abaixo;
} NoPilha;

typedef struct {
    NoPilha *topo;
    int tamanho;
} PilhaEncadeada;

void pilha_enc_criar(PilhaEncadeada *p);
bool pilha_enc_empilhar(PilhaEncadeada *p, Prioridade item);      /* false se malloc falhar */
bool pilha_enc_desempilhar(PilhaEncadeada *p, Prioridade *saida);
bool pilha_enc_topo(const PilhaEncadeada *p, Prioridade *saida);
bool pilha_enc_esta_vazia(const PilhaEncadeada *p);
int  pilha_enc_tamanho(const PilhaEncadeada *p);
void pilha_enc_destruir(PilhaEncadeada *p);

#endif
