/*
 * undo_redo_reclassificacao.c
 * Todas as operacoes sao O(1): apenas empilhar/desempilhar.
 */
#include "undo_redo_reclassificacao.h"

void gerenciador_iniciar(GerenciadorReclassificacao *g, Voo *voo)
{
    g->voo = voo;
    pilha_array_criar(&g->desfazer);
    pilha_array_criar(&g->refazer);
}

bool gerenciador_reclassificar(GerenciadorReclassificacao *g, Prioridade nova)
{
    if (!pilha_array_empilhar(&g->desfazer, g->voo->prioridade))
        return false;
    g->voo->prioridade = nova;

    /* Uma acao nova invalida o "futuro" do refazer (igual editor de texto). */
    pilha_array_criar(&g->refazer);
    return true;
}

bool gerenciador_desfazer(GerenciadorReclassificacao *g)
{
    Prioridade anterior;

    if (!pilha_array_desempilhar(&g->desfazer, &anterior))
        return false;
    pilha_array_empilhar(&g->refazer, g->voo->prioridade);  /* guarda a atual */
    g->voo->prioridade = anterior;
    return true;
}

bool gerenciador_refazer(GerenciadorReclassificacao *g)
{
    Prioridade proxima;

    if (!pilha_array_desempilhar(&g->refazer, &proxima))
        return false;
    pilha_array_empilhar(&g->desfazer, g->voo->prioridade);
    g->voo->prioridade = proxima;
    return true;
}

bool gerenciador_pode_desfazer(const GerenciadorReclassificacao *g)
{
    return !pilha_array_esta_vazia(&g->desfazer);
}

bool gerenciador_pode_refazer(const GerenciadorReclassificacao *g)
{
    return !pilha_array_esta_vazia(&g->refazer);
}
