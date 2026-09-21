/*
 * repositorio_dinamico.h
 * Semana 6 - TAD Repositorio de Voos com alocacao DINAMICA.
 *
 * Cada voo fica em um no alocado com malloc() no momento da insercao
 * e liberado com free() na remocao. Nao ha limite de capacidade alem
 * da memoria disponivel.
 */
#ifndef REPOSITORIO_DINAMICO_H
#define REPOSITORIO_DINAMICO_H

#include <stdbool.h>
#include "voo.h"

typedef struct NoVoo {
    Voo voo;
    struct NoVoo *proximo;
} NoVoo;

typedef struct {
    NoVoo *inicio;     /* NULL quando vazio */
    int quantidade;
} RepositorioDinamico;

/* 'capacidade' e ignorada (so existe para manter a mesma interface). */
bool repo_dinamico_criar(RepositorioDinamico *repo, int capacidade);
bool repo_dinamico_inserir(RepositorioDinamico *repo, Voo voo);   /* false so se malloc falhar */
bool repo_dinamico_remover(RepositorioDinamico *repo, int id);
Voo *repo_dinamico_buscar(RepositorioDinamico *repo, int id);
int  repo_dinamico_listar(const RepositorioDinamico *repo, Voo destino[], int max);
bool repo_dinamico_esta_cheio(const RepositorioDinamico *repo);   /* sempre false */
int  repo_dinamico_quantidade(const RepositorioDinamico *repo);
void repo_dinamico_destruir(RepositorioDinamico *repo);           /* free em todos os nos */

#endif
