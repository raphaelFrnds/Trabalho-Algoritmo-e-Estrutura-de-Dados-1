/*
 * lista_dupla_circular.c
 */
#include <stdlib.h>
#include "lista_dupla_circular.h"

void lista_dupla_criar(ListaDupla *l, bool circular)
{
    l->inicio = l->fim = NULL;
    l->tamanho = 0;
    l->circular = circular;
}

/* Liga (ou desliga) as pontas depois de cada insercao/remocao. */
static void ajustar_circularidade(ListaDupla *l)
{
    if (l->tamanho == 0)
        return;
    if (l->circular) {
        l->fim->proximo = l->inicio;
        l->inicio->anterior = l->fim;
    } else {
        l->fim->proximo = NULL;
        l->inicio->anterior = NULL;
    }
}

/* Anda a partir da ponta mais proxima (inicio ou fim). */
static NoDuplo *no_na_posicao(const ListaDupla *l, int posicao)
{
    NoDuplo *no;
    if (posicao < l->tamanho / 2) {
        no = l->inicio;
        for (int i = 0; i < posicao; i++)
            no = no->proximo;
    } else {
        no = l->fim;
        for (int i = l->tamanho - 1; i > posicao; i--)
            no = no->anterior;
    }
    return no;
}

/* Pontas: O(1). Meio: O(n). */
bool lista_dupla_inserir(ListaDupla *l, Voo *voo, int posicao)
{
    if (posicao == POSICAO_FIM)
        posicao = l->tamanho;
    if (posicao < 0 || posicao > l->tamanho)
        return false;

    NoDuplo *novo = malloc(sizeof(NoDuplo));
    if (novo == NULL)
        return false;
    novo->voo = voo;
    novo->anterior = novo->proximo = NULL;

    if (l->tamanho == 0) {
        l->inicio = l->fim = novo;
    } else if (posicao == 0) {
        novo->proximo = l->inicio;
        l->inicio->anterior = novo;
        l->inicio = novo;
    } else if (posicao == l->tamanho) {
        novo->anterior = l->fim;
        l->fim->proximo = novo;
        l->fim = novo;
    } else {
        NoDuplo *atual = no_na_posicao(l, posicao);
        novo->anterior = atual->anterior;
        novo->proximo = atual;
        atual->anterior->proximo = novo;
        atual->anterior = novo;
    }
    l->tamanho++;
    ajustar_circularidade(l);
    return true;
}

/* Pontas: O(1), inclusive a ultima (gracas ao 'anterior'). Meio: O(n). */
bool lista_dupla_remover(ListaDupla *l, int posicao, Voo **removido)
{
    if (posicao < 0 || posicao >= l->tamanho)
        return false;

    NoDuplo *alvo = no_na_posicao(l, posicao);

    if (l->tamanho == 1) {
        l->inicio = l->fim = NULL;
    } else {
        if (alvo == l->inicio) l->inicio = alvo->proximo;
        if (alvo == l->fim)    l->fim = alvo->anterior;
        /* religa os vizinhos, pulando o no removido */
        if (alvo->anterior != NULL) alvo->anterior->proximo = alvo->proximo;
        if (alvo->proximo != NULL)  alvo->proximo->anterior = alvo->anterior;
    }

    if (removido != NULL)
        *removido = alvo->voo;
    free(alvo);
    l->tamanho--;
    ajustar_circularidade(l);
    return true;
}

/* Percorre exatamente 'tamanho' nos, para nao entrar em loop infinito
 * quando a lista e circular. */
int lista_dupla_buscar(const ListaDupla *l, int id_voo)
{
    NoDuplo *no = l->inicio;
    for (int i = 0; i < l->tamanho; i++, no = no->proximo)
        if (no->voo->id == id_voo)
            return i;
    return -1;
}

int lista_dupla_percorrer(const ListaDupla *l, Voo *destino[], int max)
{
    int n = 0;
    NoDuplo *no = l->inicio;
    for (int i = 0; i < l->tamanho && n < max; i++, no = no->proximo)
        destino[n++] = no->voo;
    return n;
}

int lista_dupla_percorrer_reverso(const ListaDupla *l, Voo *destino[], int max)
{
    int n = 0;
    NoDuplo *no = l->fim;
    for (int i = 0; i < l->tamanho && n < max; i++, no = no->anterior)
        destino[n++] = no->voo;
    return n;
}

int lista_dupla_tamanho(const ListaDupla *l)   { return l->tamanho; }
bool lista_dupla_esta_vazia(const ListaDupla *l) { return l->tamanho == 0; }

void lista_dupla_destruir(ListaDupla *l)
{
    NoDuplo *no = l->inicio;
    for (int i = 0; i < l->tamanho; i++) {
        NoDuplo *proximo = no->proximo;
        free(no);
        no = proximo;
    }
    lista_dupla_criar(l, l->circular);
}
