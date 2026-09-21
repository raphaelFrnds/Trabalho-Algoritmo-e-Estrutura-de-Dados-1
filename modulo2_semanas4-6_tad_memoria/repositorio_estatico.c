/*
 * repositorio_estatico.c
 */
#include <stddef.h>
#include "repositorio_estatico.h"

bool repo_estatico_criar(RepositorioEstatico *repo, int capacidade)
{
    if (capacidade <= 0 || capacidade > CAPACIDADE_MAX)
        return false;
    repo->capacidade = capacidade;
    repo->quantidade = 0;
    for (int i = 0; i < CAPACIDADE_MAX; i++)
        repo->ocupado[i] = false;
    return true;
}

bool repo_estatico_esta_cheio(const RepositorioEstatico *repo)
{
    return repo->quantidade >= repo->capacidade;
}

int repo_estatico_quantidade(const RepositorioEstatico *repo)
{
    return repo->quantidade;
}

/* O(n) no pior caso: procura a primeira posicao livre. */
bool repo_estatico_inserir(RepositorioEstatico *repo, Voo voo)
{
    if (repo_estatico_esta_cheio(repo))
        return false;
    for (int i = 0; i < repo->capacidade; i++) {
        if (!repo->ocupado[i]) {
            repo->voos[i] = voo;          /* copia a struct para dentro do vetor */
            repo->ocupado[i] = true;
            repo->quantidade++;
            return true;
        }
    }
    return false;
}

/* O(n): varre o vetor procurando o id. So marca a posicao como livre. */
bool repo_estatico_remover(RepositorioEstatico *repo, int id)
{
    for (int i = 0; i < repo->capacidade; i++) {
        if (repo->ocupado[i] && repo->voos[i].id == id) {
            repo->ocupado[i] = false;
            repo->quantidade--;
            return true;
        }
    }
    return false;
}

/* O(n): busca linear (o vetor nao esta ordenado por id). */
Voo *repo_estatico_buscar(RepositorioEstatico *repo, int id)
{
    for (int i = 0; i < repo->capacidade; i++)
        if (repo->ocupado[i] && repo->voos[i].id == id)
            return &repo->voos[i];
    return NULL;
}

/* O(n): copia so as posicoes ocupadas. */
int repo_estatico_listar(const RepositorioEstatico *repo, Voo destino[], int max)
{
    int n = 0;
    for (int i = 0; i < repo->capacidade && n < max; i++)
        if (repo->ocupado[i])
            destino[n++] = repo->voos[i];
    return n;
}
