/*
 * lista_array.h
 * Semana 7 - Lista sequencial (vetor).
 * Guarda PONTEIROS para Voo: a lista nao e dona dos voos, so organiza
 * a ordem deles (quem cria o voo e quem cuida da memoria dele).
 */
#ifndef LISTA_ARRAY_H
#define LISTA_ARRAY_H

#include <stdbool.h>
#include "voo.h"

#define LISTA_CAP 100
#ifndef POSICAO_FIM
#define POSICAO_FIM (-1)   /* use como 'posicao' para inserir no final */
#endif

typedef struct {
    Voo *itens[LISTA_CAP];
    int tamanho;
} ListaArray;

void lista_array_criar(ListaArray *l);
/* false se a lista estiver cheia ou a posicao for invalida */
bool lista_array_inserir(ListaArray *l, Voo *voo, int posicao);
/* Remove da posicao; se 'removido' != NULL, devolve o voo por ele */
bool lista_array_remover(ListaArray *l, int posicao, Voo **removido);
/* Posicao do voo com esse id, ou -1 */
int  lista_array_buscar(const ListaArray *l, int id_voo);
/* Copia os ponteiros em ordem para 'destino'; retorna quantos copiou */
int  lista_array_percorrer(const ListaArray *l, Voo *destino[], int max);
int  lista_array_tamanho(const ListaArray *l);
bool lista_array_esta_vazia(const ListaArray *l);

#endif
