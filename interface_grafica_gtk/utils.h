/*
 * utils.h
 * Funcoes utilitarias gerais: geracao de IDs e formatacao de data/hora.
 */
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <time.h>

/* Gera um ID sequencial unico (1, 2, 3...) a cada chamada. */
int gerar_id_voo(void);

/* Volta o contador para 1. Usado apenas nos testes. */
void resetar_contador_id(void);

/*
 * Escreve em 'destino' a data/hora no formato "DD/MM/AAAA HH:MM:SS".
 * Se 'momento' for NULL, usa o instante atual.
 */
void formatar_hora_chegada(char *destino, size_t tam, const struct tm *momento);

#endif
