/*
 * lista_dupla_circular.h
 * Semana 9 - Lista duplamente encadeada e lista circular.
 *
 * Cada no aponta para o anterior e para o proximo. Com circular=true,
 * o proximo do ultimo volta para o primeiro (e o anterior do primeiro
 * aponta para o ultimo) - util para rodizio, ex.: escala de pistas
 * ou de controladores no turno.
 */
#ifndef LISTA_DUPLA_CIRCULAR_H
#define LISTA_DUPLA_CIRCULAR_H

#include <stdbool.h>
#include "voo.h"

#ifndef POSICAO_FIM
#define POSICAO_FIM (-1)
#endif

typedef struct NoDuplo {
    Voo *voo;
    struct NoDuplo *anterior;
    struct NoDuplo *proximo;
} NoDuplo;

typedef struct {
    NoDuplo *inicio;
    NoDuplo *fim;
    int tamanho;
    bool circular;
} ListaDupla;

void lista_dupla_criar(ListaDupla *l, bool circular);
bool lista_dupla_inserir(ListaDupla *l, Voo *voo, int posicao);
bool lista_dupla_remover(ListaDupla *l, int posicao, Voo **removido);
int  lista_dupla_buscar(const ListaDupla *l, int id_voo);
/* Do inicio para o fim */
int  lista_dupla_percorrer(const ListaDupla *l, Voo *destino[], int max);
/* Do fim para o inicio (usa os ponteiros 'anterior') */
int  lista_dupla_percorrer_reverso(const ListaDupla *l, Voo *destino[], int max);
int  lista_dupla_tamanho(const ListaDupla *l);
bool lista_dupla_esta_vazia(const ListaDupla *l);
void lista_dupla_destruir(ListaDupla *l);

#endif
