/*
 * main.c
 * Ponto de entrada do Entregavel 1 (Modulo 1 - Semanas 1 a 3).
 * Mostra a biblioteca funcionando junta: ID, validacao, passagem de
 * parametros e recursao.
 */
#include <stdio.h>
#include <stdbool.h>
#include "voo.h"
#include "utils.h"
#include "validacao.h"
#include "parametros_demo.h"
#include "recursao_demo.h"

/* Valida os dados e preenche *saida. Retorna false se algo for invalido. */
static bool cadastrar_voo(const char *codigo, const char *prioridade, Voo *saida)
{
    char hora[TAM_HORA];

    if (!validar_codigo(codigo) || !validar_prioridade(prioridade))
        return false;

    formatar_hora_chegada(hora, sizeof hora, NULL);
    *saida = voo_criar(gerar_id_voo(), codigo,
                       (Prioridade)prioridade_numerica(prioridade), hora);
    return true;
}

int main(void)
{
    Voo voos[3];

    printf("=== Controle de Pousos - Entregavel 1 (Modulo 1) ===\n\n");

    cadastrar_voo("AZU4521", "regular", &voos[0]);
    cadastrar_voo("GLO1180", "combustivel", &voos[1]);
    cadastrar_voo("TAM3301", "mayday", &voos[2]);

    for (int i = 0; i < 3; i++) {
        voo_imprimir(&voos[i]);
        printf("  prioridade numerica: %d\n", (int)voos[i].prioridade);
    }

    Voo invalido;
    printf("\nTentando cadastrar com prioridade 'urgente': %s\n",
           cadastrar_voo("XYZ1", "urgente", &invalido) ? "aceito" : "rejeitado");

    printf("\nPor valor: tentar_reclassificar_copia(voos[0], panpan)\n");
    tentar_reclassificar_copia(voos[0], PRIO_PANPAN);
    voo_imprimir(&voos[0]);
    printf("  (nao mudou: a funcao recebeu uma copia)\n");

    printf("\nPor referencia: reclassificar_voo(&voos[0], panpan)\n");
    reclassificar_voo(&voos[0], PRIO_PANPAN);
    voo_imprimir(&voos[0]);
    printf("  (mudou: a funcao recebeu o endereco)\n");

    int x = 10, y = 20;
    trocar_por_valor(x, y);
    printf("\nApos trocar_por_valor(x, y): x=%d, y=%d\n", x, y);
    trocar_valores(&x, &y);
    printf("Apos trocar_valores(&x, &y): x=%d, y=%d\n", x, y);

    printf("\n5! = %lld\n", fatorial(5));
    printf("Fibonacci(10) = %lld\n", fibonacci(10));

    int idx = busca_binaria_recursiva(voos, 2, 0, 2);
    printf("Busca binaria do id=2 -> indice %d: ", idx);
    if (idx != -1)
        voo_imprimir(&voos[idx]);
    return 0;
}
