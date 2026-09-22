/*
 * lista_array.c
 */
#include "lista_array.h"

void lista_array_criar(ListaArray *l)
{
    l->tamanho = 0;
}

/* Final: O(1). Inicio/meio: O(n), porque desloca os seguintes para a direita. */
bool lista_array_inserir(ListaArray *l, Voo *voo, int posicao)
{
    if (posicao == POSICAO_FIM)
        posicao = l->tamanho;
    if (l->tamanho == LISTA_CAP || posicao < 0 || posicao > l->tamanho)
        return false;

    for (int i = l->tamanho; i > posicao; i--)
        l->itens[i] = l->itens[i - 1];
    l->itens[posicao] = voo;
    l->tamanho++;
    return true;
}

/* O(n): desloca os seguintes para a esquerda (O(1) se for o ultimo). */
bool lista_array_remover(ListaArray *l, int posicao, Voo **removido)
{
    if (posicao < 0 || posicao >= l->tamanho)
        return false;
    if (removido != NULL)
        *removido = l->itens[posicao];

    for (int i = posicao; i < l->tamanho - 1; i++)
        l->itens[i] = l->itens[i + 1];
    l->tamanho--;
    return true;
}

/* O(n): busca linear. */
int lista_array_buscar(const ListaArray *l, int id_voo)
{
    for (int i = 0; i < l->tamanho; i++)
        if (l->itens[i]->id == id_voo)
            return i;
    return -1;
}

int lista_array_percorrer(const ListaArray *l, Voo *destino[], int max)
{
    int n = 0;
    for (int i = 0; i < l->tamanho && n < max; i++)
        destino[n++] = l->itens[i];
    return n;
}

int lista_array_tamanho(const ListaArray *l)   { return l->tamanho; }
bool lista_array_esta_vazia(const ListaArray *l) { return l->tamanho == 0; }
