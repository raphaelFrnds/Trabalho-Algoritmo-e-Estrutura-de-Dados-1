/*
 * voo.c
 * Implementacao das funcoes basicas da entidade Voo.
 */
#include <stdio.h>
#include <string.h>
#include "voo.h"
#include "validacao.h"

Voo voo_criar(int id, const char *codigo, Prioridade prioridade, const char *hora)
{
    Voo v;
    v.id = id;
    v.prioridade = prioridade;
    /* snprintf garante o '\0' no final mesmo se a string for longa demais */
    snprintf(v.codigo, sizeof v.codigo, "%s", codigo);
    snprintf(v.hora_chegada, sizeof v.hora_chegada, "%s", hora);
    return v;
}

void voo_imprimir(const Voo *voo)
{
    printf("Voo(id=%d, %s, %s, %s)\n", voo->id, voo->codigo,
           nome_prioridade(voo->prioridade), voo->hora_chegada);
}
