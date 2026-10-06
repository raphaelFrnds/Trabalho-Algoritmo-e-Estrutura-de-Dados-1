/*
 * fila_encadeada.c
 */
#include <stdlib.h>
#include "fila_encadeada.h"

void fila_enc_criar(FilaEncadeada *f)
{
    f->inicio = f->fim = NULL;
    f->quantidade = 0;
}

/* O(1): gracas ao ponteiro 'fim' */
bool fila_enc_enfileirar(FilaEncadeada *f, Voo *voo)
{
    NoFila *novo = malloc(sizeof(NoFila));
    if (novo == NULL)
        return false;
    novo->voo = voo;
    novo->proximo = NULL;

    if (f->fim == NULL)
        f->inicio = novo;          /* fila estava vazia */
    else
        f->fim->proximo = novo;
    f->fim = novo;
    f->quantidade++;
    return true;
}

/* O(1): remove do inicio */
bool fila_enc_desenfileirar(FilaEncadeada *f, Voo **saida)
{
    if (fila_enc_esta_vazia(f))
        return false;
    NoFila *removido = f->inicio;
    *saida = removido->voo;
    f->inicio = removido->proximo;
    if (f->inicio == NULL)
        f->fim = NULL;             /* ficou vazia */
    free(removido);
    f->quantidade--;
    return true;
}

Voo *fila_enc_frente(const FilaEncadeada *f)
{
    return fila_enc_esta_vazia(f) ? NULL : f->inicio->voo;
}

bool fila_enc_esta_vazia(const FilaEncadeada *f) { return f->inicio == NULL; }
int  fila_enc_tamanho(const FilaEncadeada *f)    { return f->quantidade; }

int fila_enc_posicao(const FilaEncadeada *f, int id_voo)
{
    int posicao = 0;
    for (NoFila *no = f->inicio; no != NULL; no = no->proximo, posicao++)
        if (no->voo->id == id_voo)
            return posicao;
    return -1;
}

void fila_enc_destruir(FilaEncadeada *f)
{
    Voo *descartado;
    while (fila_enc_desenfileirar(f, &descartado))
        ;
}
