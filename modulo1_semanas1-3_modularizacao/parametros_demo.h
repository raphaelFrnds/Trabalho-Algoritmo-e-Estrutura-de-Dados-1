/*
 * parametros_demo.h
 * Semana 2 - Passagem de parametros por valor e por referencia.
 *
 * Em C a diferenca e explicita:
 *   - por VALOR: a funcao recebe uma COPIA (ex.: Voo voo). Alterar a
 *     copia nao muda a variavel de quem chamou.
 *   - por REFERENCIA: a funcao recebe o ENDERECO (ex.: Voo *voo) e
 *     altera a variavel original atraves do ponteiro.
 */
#ifndef PARAMETROS_DEMO_H
#define PARAMETROS_DEMO_H

#include "voo.h"

/* Por referencia: altera o voo original. */
void reclassificar_voo(Voo *voo, Prioridade nova);

/* Por valor: altera so a copia local; devolve a prioridade da copia. */
Prioridade tentar_reclassificar_copia(Voo voo, Prioridade nova);

/* Swap por referencia: troca de verdade os valores de *a e *b. */
void trocar_valores(int *a, int *b);

/* Swap por valor: NAO funciona, troca apenas as copias locais. */
void trocar_por_valor(int a, int b);

#endif
