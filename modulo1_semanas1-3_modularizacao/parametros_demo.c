/*
 * parametros_demo.c
 */
#include "parametros_demo.h"

void reclassificar_voo(Voo *voo, Prioridade nova)
{
    voo->prioridade = nova;   /* escreve no endereco original */
}

Prioridade tentar_reclassificar_copia(Voo voo, Prioridade nova)
{
    voo.prioridade = nova;    /* 'voo' e uma copia: o original nao muda */
    return voo.prioridade;
}

void trocar_valores(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void trocar_por_valor(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
    (void)a;                  /* evita aviso de "variavel nao usada" */
    (void)b;
}
