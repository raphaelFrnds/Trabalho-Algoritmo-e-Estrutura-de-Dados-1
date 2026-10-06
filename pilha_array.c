/*
 * pilha_array.c
 * Todas as operacoes sao O(1): so mexem no final do vetor.
 */
#include "pilha_array.h"

void pilha_array_criar(PilhaArray *p)
{
    p->topo = 0;
}

bool pilha_array_empilhar(PilhaArray *p, Prioridade item)
{
    if (pilha_array_esta_cheia(p))
        return false;
    p->itens[p->topo++] = item;
    return true;
}

bool pilha_array_desempilhar(PilhaArray *p, Prioridade *saida)
{
    if (pilha_array_esta_vazia(p))
        return false;
    *saida = p->itens[--p->topo];
    return true;
}

bool pilha_array_topo(const PilhaArray *p, Prioridade *saida)
{
    if (pilha_array_esta_vazia(p))
        return false;
    *saida = p->itens[p->topo - 1];
    return true;
}

bool pilha_array_esta_vazia(const PilhaArray *p) { return p->topo == 0; }
bool pilha_array_esta_cheia(const PilhaArray *p) { return p->topo == PILHA_CAP; }
int  pilha_array_tamanho(const PilhaArray *p)    { return p->topo; }
