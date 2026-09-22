/*
 * fila_array_circular.h
 * Semana 12 - Fila (FIFO) com VETOR CIRCULAR de capacidade fixa.
 * O indice "da a volta" com o operador %, reaproveitando as posicoes
 * liberadas no inicio do vetor - desenfileirar fica O(1), sem deslocar.
 */
#ifndef FILA_ARRAY_CIRCULAR_H
#define FILA_ARRAY_CIRCULAR_H

#include <stdbool.h>
#include "voo.h"

typedef struct {
    Voo **itens;       /* vetor alocado UMA vez, com 'capacidade' posicoes */
    int capacidade;
    int inicio;        /* indice da frente da fila */
    int quantidade;
} FilaCircular;

bool fila_circ_criar(FilaCircular *f, int capacidade);   /* false se capacidade <= 0 */
bool fila_circ_enfileirar(FilaCircular *f, Voo *voo);    /* false se cheia */
bool fila_circ_desenfileirar(FilaCircular *f, Voo **saida);
Voo *fila_circ_frente(const FilaCircular *f);            /* NULL se vazia */
bool fila_circ_esta_vazia(const FilaCircular *f);
bool fila_circ_esta_cheia(const FilaCircular *f);
int  fila_circ_tamanho(const FilaCircular *f);
void fila_circ_destruir(FilaCircular *f);

#endif
