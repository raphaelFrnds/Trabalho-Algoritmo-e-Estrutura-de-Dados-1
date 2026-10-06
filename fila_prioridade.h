/*
 * fila_prioridade.h
 * Semana 13 - Fila de prioridade de POUSO (5 niveis).
 * E o COMPONENTE CENTRAL do sistema: um voo 'mayday' pousa antes de
 * qualquer outro, mesmo que tenha chegado depois.
 *
 * Implementacao: um vetor com 5 FilaEncadeada (Semana 12), uma por
 * nivel. O indice do vetor e (nivel - 1), entao:
 *   1. entre niveis diferentes, o mais urgente sai primeiro;
 *   2. dentro do MESMO nivel, vale a ordem de chegada (FIFO) - dois
 *      voos 'regular' pousam na ordem em que chegaram.
 */
#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

#include <stdbool.h>
#include "voo.h"
#include "fila_encadeada.h"

typedef struct {
    FilaEncadeada niveis[NUM_PRIORIDADES];   /* niveis[0] = mayday ... niveis[4] = treinamento */
} FilaPrioridade;

void fila_prio_criar(FilaPrioridade *fp);
/* O(1). false se a prioridade do voo for invalida. */
bool fila_prio_enfileirar(FilaPrioridade *fp, Voo *voo);
/* O(k), k = 5. Tira o voo mais urgente que chegou primeiro. false se vazia. */
bool fila_prio_desenfileirar(FilaPrioridade *fp, Voo **saida);
/* Consulta o proximo a pousar, sem remover. NULL se vazia. */
Voo *fila_prio_proximo(const FilaPrioridade *fp);
bool fila_prio_esta_vazia(const FilaPrioridade *fp);
int  fila_prio_tamanho(const FilaPrioridade *fp);
/* Preenche contagem[0..4] com a quantidade de voos em cada nivel. */
void fila_prio_tamanho_por_nivel(const FilaPrioridade *fp, int contagem[NUM_PRIORIDADES]);
/* Posicao estimada na sequencia de pouso (0 = proximo) ou -1. O(n). */
int  fila_prio_posicao_estimada(const FilaPrioridade *fp, int id_voo);
void fila_prio_destruir(FilaPrioridade *fp);

#endif
