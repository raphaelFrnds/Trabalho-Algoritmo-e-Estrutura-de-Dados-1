/*
 * fila_array_circular.c
 */
#include <stdlib.h>
#include "fila_array_circular.h"

bool fila_circ_criar(FilaCircular *f, int capacidade)
{
    if (capacidade <= 0)
        return false;
    f->itens = malloc(capacidade * sizeof(Voo *));
    if (f->itens == NULL)
        return false;
    f->capacidade = capacidade;
    f->inicio = 0;
    f->quantidade = 0;
    return true;
}

/* O(1): escreve logo depois do ultimo, dando a volta com % */
bool fila_circ_enfileirar(FilaCircular *f, Voo *voo)
{
    if (fila_circ_esta_cheia(f))
        return false;
    int posicao = (f->inicio + f->quantidade) % f->capacidade;
    f->itens[posicao] = voo;
    f->quantidade++;
    return true;
}

/* O(1): so avanca o indice de inicio */
bool fila_circ_desenfileirar(FilaCircular *f, Voo **saida)
{
    if (fila_circ_esta_vazia(f))
        return false;
    *saida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->quantidade--;
    return true;
}

Voo *fila_circ_frente(const FilaCircular *f)
{
    return fila_circ_esta_vazia(f) ? NULL : f->itens[f->inicio];
}

bool fila_circ_esta_vazia(const FilaCircular *f) { return f->quantidade == 0; }
bool fila_circ_esta_cheia(const FilaCircular *f) { return f->quantidade == f->capacidade; }
int  fila_circ_tamanho(const FilaCircular *f)    { return f->quantidade; }

void fila_circ_destruir(FilaCircular *f)
{
    free(f->itens);
    f->itens = NULL;
    f->capacidade = f->quantidade = f->inicio = 0;
}
