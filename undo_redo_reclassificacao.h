/*
 * undo_redo_reclassificacao.h
 * Semana 11 - Aplicacao de pilha: desfazer/refazer reclassificacoes
 * de prioridade de um voo.
 *
 * Cenario: o controlador muda um voo para 'panpan' por engano (era
 * 'combustivel') e precisa voltar atras rapido - sem perder a chance
 * de refazer, caso mude de ideia de novo.
 *
 * Usa DUAS pilhas:
 *   desfazer: prioridades ANTERIORES (historico)
 *   refazer:  prioridades que foram desfeitas
 */
#ifndef UNDO_REDO_RECLASSIFICACAO_H
#define UNDO_REDO_RECLASSIFICACAO_H

#include <stdbool.h>
#include "voo.h"
#include "pilha_array.h"

typedef struct {
    Voo *voo;               /* voo controlado (alterado por ponteiro) */
    PilhaArray desfazer;
    PilhaArray refazer;
} GerenciadorReclassificacao;

void gerenciador_iniciar(GerenciadorReclassificacao *g, Voo *voo);

/* Aplica a nova prioridade e guarda a anterior. Limpa o 'refazer'.
 * Retorna false se o historico estiver cheio. */
bool gerenciador_reclassificar(GerenciadorReclassificacao *g, Prioridade nova);

/* Volta para a prioridade anterior. false se nao houver o que desfazer. */
bool gerenciador_desfazer(GerenciadorReclassificacao *g);

/* Reaplica a ultima prioridade desfeita. false se nao houver o que refazer. */
bool gerenciador_refazer(GerenciadorReclassificacao *g);

bool gerenciador_pode_desfazer(const GerenciadorReclassificacao *g);
bool gerenciador_pode_refazer(const GerenciadorReclassificacao *g);

#endif
