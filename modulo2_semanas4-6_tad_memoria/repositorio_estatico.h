/*
 * repositorio_estatico.h
 * Semana 5 - TAD Repositorio de Voos com alocacao ESTATICA.
 *
 * O vetor 'voos' tem tamanho fixo definido em tempo de COMPILACAO
 * (CAPACIDADE_MAX). A struct inteira pode ficar na pilha de execucao
 * (variavel local) ou em memoria global - nenhum malloc e usado.
 */
#ifndef REPOSITORIO_ESTATICO_H
#define REPOSITORIO_ESTATICO_H

#include <stdbool.h>
#include "voo.h"

#define CAPACIDADE_MAX 100

typedef struct {
    Voo voos[CAPACIDADE_MAX];        /* espaco reservado de uma vez */
    bool ocupado[CAPACIDADE_MAX];    /* marca quais posicoes estao em uso */
    int capacidade;                  /* limite escolhido (<= CAPACIDADE_MAX) */
    int quantidade;
} RepositorioEstatico;

/* Retorna false se capacidade <= 0 ou > CAPACIDADE_MAX. */
bool repo_estatico_criar(RepositorioEstatico *repo, int capacidade);
bool repo_estatico_inserir(RepositorioEstatico *repo, Voo voo);   /* false se cheio */
bool repo_estatico_remover(RepositorioEstatico *repo, int id);    /* false se nao achar */
Voo *repo_estatico_buscar(RepositorioEstatico *repo, int id);     /* NULL se nao achar */
int  repo_estatico_listar(const RepositorioEstatico *repo, Voo destino[], int max);
bool repo_estatico_esta_cheio(const RepositorioEstatico *repo);
int  repo_estatico_quantidade(const RepositorioEstatico *repo);

#endif
