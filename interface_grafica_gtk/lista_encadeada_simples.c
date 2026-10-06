/*
 * lista_encadeada_simples.c
 */
#include <stdlib.h>
#include "lista_encadeada_simples.h"

void lista_simples_criar(ListaSimples *l)
{
    l->inicio = l->fim = NULL;
    l->tamanho = 0;
}

/* Inicio e final: O(1). Meio: O(n), porque precisa andar ate a posicao. */
bool lista_simples_inserir(ListaSimples *l, Voo *voo, int posicao)
{
    if (posicao == POSICAO_FIM)
        posicao = l->tamanho;
    if (posicao < 0 || posicao > l->tamanho)
        return false;

    NoSimples *novo = malloc(sizeof(NoSimples));
    if (novo == NULL)
        return false;
    novo->voo = voo;
    novo->proximo = NULL;

    if (posicao == 0) {                        /* novo inicio */
        novo->proximo = l->inicio;
        l->inicio = novo;
        if (l->tamanho == 0)
            l->fim = novo;
    } else if (posicao == l->tamanho) {        /* novo fim */
        l->fim->proximo = novo;
        l->fim = novo;
    } else {                                   /* no meio */
        NoSimples *anterior = l->inicio;
        for (int i = 0; i < posicao - 1; i++)
            anterior = anterior->proximo;
        novo->proximo = anterior->proximo;
        anterior->proximo = novo;
    }
    l->tamanho++;
    return true;
}

/* Inicio: O(1). Qualquer outra posicao (inclusive a ultima): O(n),
 * porque so da para chegar no no ANTERIOR andando desde o inicio. */
bool lista_simples_remover(ListaSimples *l, int posicao, Voo **removido)
{
    if (posicao < 0 || posicao >= l->tamanho)
        return false;

    NoSimples *alvo;
    if (posicao == 0) {
        alvo = l->inicio;
        l->inicio = alvo->proximo;
        if (l->inicio == NULL)
            l->fim = NULL;
    } else {
        NoSimples *anterior = l->inicio;
        for (int i = 0; i < posicao - 1; i++)
            anterior = anterior->proximo;
        alvo = anterior->proximo;
        anterior->proximo = alvo->proximo;
        if (alvo == l->fim)
            l->fim = anterior;
    }

    if (removido != NULL)
        *removido = alvo->voo;
    free(alvo);
    l->tamanho--;
    return true;
}

int lista_simples_buscar(const ListaSimples *l, int id_voo)
{
    int posicao = 0;
    for (NoSimples *no = l->inicio; no != NULL; no = no->proximo, posicao++)
        if (no->voo->id == id_voo)
            return posicao;
    return -1;
}

Voo *lista_simples_obter(const ListaSimples *l, int posicao)
{
    if (posicao < 0 || posicao >= l->tamanho)
        return NULL;
    NoSimples *no = l->inicio;
    for (int i = 0; i < posicao; i++)
        no = no->proximo;
    return no->voo;
}

int lista_simples_percorrer(const ListaSimples *l, Voo *destino[], int max)
{
    int n = 0;
    for (NoSimples *no = l->inicio; no != NULL && n < max; no = no->proximo)
        destino[n++] = no->voo;
    return n;
}

int lista_simples_tamanho(const ListaSimples *l)   { return l->tamanho; }
bool lista_simples_esta_vazia(const ListaSimples *l) { return l->tamanho == 0; }

void lista_simples_destruir(ListaSimples *l)
{
    NoSimples *no = l->inicio;
    while (no != NULL) {
        NoSimples *proximo = no->proximo;
        free(no);
        no = proximo;
    }
    lista_simples_criar(l);
}
