/*
 * validacao.h
 * Validacao dos dados de entrada (codigo do voo e prioridade).
 * Fica separada de utils.c e de voo.c: cada modulo tem uma unica
 * responsabilidade (Semana 1 - modularizacao).
 */
#ifndef VALIDACAO_H
#define VALIDACAO_H

#include <stdbool.h>
#include "voo.h"

/* Uma linha da tabela de prioridades de pouso. */
typedef struct {
    const char *nome;        /* texto digitado pelo usuario, ex.: "mayday" */
    const char *descricao;   /* texto para exibicao */
    Prioridade nivel;        /* 1 (mais urgente) a 5 */
    int espera_max_min;      /* tempo maximo de espera recomendado */
} InfoPrioridade;

/* Tabela com os 5 niveis, na ordem do mais urgente para o menos urgente. */
extern const InfoPrioridade TABELA_PRIORIDADES[NUM_PRIORIDADES];

/* true se 'nome' for um dos 5 niveis (ignora maiusculas e espacos nas pontas). */
bool validar_prioridade(const char *nome);

/* true se o codigo nao for vazio e couber em Voo.codigo. */
bool validar_codigo(const char *codigo);

/* Converte o nome no nivel numerico (1 a 5). Retorna -1 se for invalido. */
int prioridade_numerica(const char *nome);

/* Converte o nivel no nome curto ("mayday", "regular"...). */
const char *nome_prioridade(Prioridade nivel);

#endif
