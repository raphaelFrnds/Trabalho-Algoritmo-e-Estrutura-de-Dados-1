/*
 * repositorio_dinamico.c
 */
#include <stdlib.h>
#include "repositorio_dinamico.h"

bool repo_dinamico_criar(RepositorioDinamico *repo, int capacidade)
{
    (void)capacidade;
    repo->inicio = NULL;
    repo->quantidade = 0;
    return true;
}

bool repo_dinamico_esta_cheio(const RepositorioDinamico *repo)
{
    (void)repo;
    return false;               /* limitado apenas pela memoria */
}

int repo_dinamico_quantidade(const RepositorioDinamico *repo)
{
    return repo->quantidade;
}

/* O(1): insere sempre no inicio da lista. */
bool repo_dinamico_inserir(RepositorioDinamico *repo, Voo voo)
{
    NoVoo *novo = malloc(sizeof(NoVoo));
    if (novo == NULL)
        return false;           /* sem memoria */

    novo->voo = voo;
    novo->proximo = repo->inicio;
    repo->inicio = novo;
    repo->quantidade++;
    return true;
}

/* O(n): percorre ate achar o id, religa os ponteiros e libera o no. */
bool repo_dinamico_remover(RepositorioDinamico *repo, int id)
{
    NoVoo *anterior = NULL;
    NoVoo *atual = repo->inicio;

    while (atual != NULL) {
        if (atual->voo.id == id) {
            if (anterior == NULL)
                repo->inicio = atual->proximo;
            else
                anterior->proximo = atual->proximo;
            free(atual);        /* sem isso: vazamento de memoria */
            repo->quantidade--;
            return true;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return false;
}

/* O(n): busca linear seguindo os ponteiros 'proximo'. */
Voo *repo_dinamico_buscar(RepositorioDinamico *repo, int id)
{
    for (NoVoo *atual = repo->inicio; atual != NULL; atual = atual->proximo)
        if (atual->voo.id == id)
            return &atual->voo;
    return NULL;
}

int repo_dinamico_listar(const RepositorioDinamico *repo, Voo destino[], int max)
{
    int n = 0;
    for (NoVoo *atual = repo->inicio; atual != NULL && n < max; atual = atual->proximo)
        destino[n++] = atual->voo;
    return n;
}

void repo_dinamico_destruir(RepositorioDinamico *repo)
{
    NoVoo *atual = repo->inicio;
    while (atual != NULL) {
        NoVoo *proximo = atual->proximo;   /* guarda antes de liberar */
        free(atual);
        atual = proximo;
    }
    repo->inicio = NULL;
    repo->quantidade = 0;
}
