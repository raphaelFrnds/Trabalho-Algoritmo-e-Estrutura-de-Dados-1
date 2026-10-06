/*
 * fila_encadeada.h
 * Semana 12 - Fila (FIFO) com nos encadeados, sem limite de capacidade.
 * Entra no fim, sai do inicio - os dois em O(1).
 */
#ifndef FILA_ENCADEADA_H
#define FILA_ENCADEADA_H

#include <stdbool.h>
#include "voo.h"

typedef struct NoFila {
    Voo *voo;
    struct NoFila *proximo;
} NoFila;

typedef struct {
    NoFila *inicio;    /* de onde sai */
    NoFila *fim;       /* onde entra */
    int quantidade;
} FilaEncadeada;

void fila_enc_criar(FilaEncadeada *f);
bool fila_enc_enfileirar(FilaEncadeada *f, Voo *voo);    /* false so se malloc falhar */
bool fila_enc_desenfileirar(FilaEncadeada *f, Voo **saida);
Voo *fila_enc_frente(const FilaEncadeada *f);            /* NULL se vazia */
bool fila_enc_esta_vazia(const FilaEncadeada *f);
int  fila_enc_tamanho(const FilaEncadeada *f);
/* Posicao (0 = frente) do voo com esse id, ou -1. O(n), nao remove nada. */
int  fila_enc_posicao(const FilaEncadeada *f, int id_voo);
/* Libera os nos (nao os voos). */
void fila_enc_destruir(FilaEncadeada *f);

#endif
