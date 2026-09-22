/*
 * simulacao_pousos.c
 * Semana 13 - Simulacao (aplicacao pedida no roteiro: "simulacao de
 * atendimento - banco, impressora, escalonamento de processos"),
 * aqui adaptada para a sequencia de pousos de uma torre de controle.
 *
 * 8 voos chegam ao espaco aereo em horario de pico e a pista libera
 * um pouso por vez, sempre respeitando a fila de prioridade.
 */
#include <stdio.h>
#include "voo.h"
#include "utils.h"
#include "validacao.h"
#include "fila_prioridade.h"

#define NUM_CHEGADAS 8

typedef struct {
    const char *codigo;
    Prioridade prioridade;
} Chegada;

int main(void)
{
    /* Ordem REAL de chegada, com prioridades variadas. */
    const Chegada chegadas[NUM_CHEGADAS] = {
        {"AZU4521", PRIO_REGULAR},
        {"GLO1180", PRIO_COMBUSTIVEL},
        {"PTB2210", PRIO_TREINAMENTO},
        {"TAM3301", PRIO_PANPAN},
        {"AZU4700", PRIO_COMBUSTIVEL},
        {"GLO1555", PRIO_MAYDAY},       /* emergencia chega no meio do fluxo */
        {"TAM3090", PRIO_REGULAR},
        {"AZU4888", PRIO_PANPAN},
    };
    Voo voos[NUM_CHEGADAS];        /* os voos vivem aqui; a fila so aponta para eles */
    Voo *pousados[NUM_CHEGADAS];   /* historico, na ordem de pouso */
    int total_pousados = 0;
    FilaPrioridade fila;
    int contagem[NUM_PRIORIDADES];

    resetar_contador_id();
    fila_prio_criar(&fila);

    printf("=== Chegada ao espaco aereo ===\n");
    for (int i = 0; i < NUM_CHEGADAS; i++) {
        voos[i] = voo_criar(gerar_id_voo(), chegadas[i].codigo,
                            chegadas[i].prioridade, "chegada simulada");
        fila_prio_enfileirar(&fila, &voos[i]);
        printf("%d. %-8s (%s)\n", i + 1, voos[i].codigo, nome_prioridade(voos[i].prioridade));
    }

    fila_prio_tamanho_por_nivel(&fila, contagem);
    printf("\nAguardando por nivel:");
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        printf(" %s=%d", TABELA_PRIORIDADES[i].nome, contagem[i]);
    printf("\n\n=== Pista liberando pousos ===\n");

    Voo *voo;
    while (fila_prio_desenfileirar(&fila, &voo)) {
        pousados[total_pousados++] = voo;
        printf("Pouso autorizado: %-8s (%s)\n", voo->codigo, nome_prioridade(voo->prioridade));
    }

    printf("\n=== Relatorio da simulacao ===\n");
    for (int i = 0; i < total_pousados; i++)
        printf("%2d. %-8s - %-12s (chegou em %do)\n", i + 1, pousados[i]->codigo,
               nome_prioridade(pousados[i]->prioridade), pousados[i]->id);

    fila_prio_destruir(&fila);
    return 0;
}
