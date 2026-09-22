/*
 * pilha_encadeada.c
 * Todas as operacoes (exceto destruir) sao O(1): so mexem no topo.
 */
#include <stdlib.h>
#include "pilha_encadeada.h"

void pilha_enc_criar(PilhaEncadeada *p)
{
    p->topo = NULL;
    p->tamanho = 0;
}

bool pilha_enc_empilhar(PilhaEncadeada *p, Prioridade item)
{
    NoPilha *novo = malloc(sizeof(NoPilha));
    if (novo == NULL)
        return false;
    novo->item = item;
    novo->abaixo = p->topo;      /* o novo no fica "em cima" do antigo topo */
    p->topo = novo;
    p->tamanho++;
    return true;
}

bool pilha_enc_desempilhar(PilhaEncadeada *p, Prioridade *saida)
{
    if (pilha_enc_esta_vazia(p))
        return false;
    NoPilha *removido = p->topo;
    *saida = removido->item;
    p->topo = removido->abaixo;
    free(removido);
    p->tamanho--;
    return true;
}

bool pilha_enc_topo(const PilhaEncadeada *p, Prioridade *saida)
{
    if (pilha_enc_esta_vazia(p))
        return false;
    *saida = p->topo->item;
    return true;
}

bool pilha_enc_esta_vazia(const PilhaEncadeada *p) { return p->topo == NULL; }
int  pilha_enc_tamanho(const PilhaEncadeada *p)    { return p->tamanho; }

void pilha_enc_destruir(PilhaEncadeada *p)
{
    Prioridade descartada;
    while (pilha_enc_desempilhar(p, &descartada))
        ;
}
