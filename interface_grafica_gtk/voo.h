/*
 * voo.h
 * Entidade Voo, usada em todo o sistema de controle de pousos.
 * No Modulo 1 e apenas uma struct simples; a partir do Modulo 2 ela
 * passa a ser guardada dentro dos TADs (repositorio, listas, filas).
 */
#ifndef VOO_H
#define VOO_H

#include <stddef.h>   /* NULL, size_t */

#define TAM_CODIGO 12   /* ex.: "AZU4521" + '\0' */
#define TAM_HORA   20   /* "DD/MM/AAAA HH:MM:SS" + '\0' */

/*
 * Os 5 niveis de prioridade de pouso (1 = mais urgente).
 * O valor numerico do enum JA E a prioridade, o que simplifica a
 * fila de prioridade do Modulo 5 (indice = nivel - 1).
 */
typedef enum {
    PRIO_MAYDAY      = 1,  /* emergencia declarada */
    PRIO_PANPAN      = 2,  /* urgencia (pane tecnica, passageiro doente) */
    PRIO_COMBUSTIVEL = 3,  /* combustivel minimo */
    PRIO_REGULAR     = 4,  /* voo comercial normal */
    PRIO_TREINAMENTO = 5   /* treinamento / translado, aceita espera */
} Prioridade;

#define NUM_PRIORIDADES 5

typedef struct {
    int id;
    char codigo[TAM_CODIGO];
    Prioridade prioridade;
    char hora_chegada[TAM_HORA];
} Voo;

/* Monta um Voo ja preenchido (copia as strings com limite de tamanho). */
Voo voo_criar(int id, const char *codigo, Prioridade prioridade, const char *hora);

/* Imprime o voo em uma linha, ex.: Voo(id=1, AZU4521, regular, 25/08/2026 10:00:00) */
void voo_imprimir(const Voo *voo);

#endif
