/*
 * interface_cli.c
 * Modulo 6 - Menu de linha de comando do Sistema de Controle de Pousos.
 * Ponto de entrada para uso interativo (demonstracao na apresentacao).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema_controle.h"
#include "validacao.h"

#define TAM_LINHA 64
#define MAX_HISTORICO 500

/* Le uma linha do teclado sem o '\n'. Retorna 0 no fim da entrada (Ctrl+D/Ctrl+Z). */
static int ler_linha(const char *mensagem, char *destino, int tam)
{
    printf("%s", mensagem);
    fflush(stdout);
    if (fgets(destino, tam, stdin) == NULL)
        return 0;
    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

/* Le um inteiro. Retorna -1 se o texto nao for um numero. */
static int ler_id(void)
{
    char linha[TAM_LINHA], *fim;
    if (!ler_linha("ID do voo: ", linha, sizeof linha))
        return -1;
    long valor = strtol(linha, &fim, 10);
    return (fim == linha) ? -1 : (int)valor;
}

static void ler_prioridade(char *destino, int tam)
{
    printf("Prioridades:");
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        printf(" %s", TABELA_PRIORIDADES[i].nome);
    printf("\n");
    if (!ler_linha("Prioridade: ", destino, tam))
        destino[0] = '\0';
}

static void exibir_menu(void)
{
    printf("\n===== TORRE DE CONTROLE - SEQUENCIA DE POUSOS =====\n");
    printf("1. Registrar voo chegando ao espaco aereo\n");
    printf("2. Autorizar proximo pouso\n");
    printf("3. Ver fila de espera (quantidade por nivel)\n");
    printf("4. Consultar posicao de um voo na fila\n");
    printf("5. Reclassificar prioridade de um voo\n");
    printf("6. Desfazer ultima reclassificacao de um voo\n");
    printf("7. Refazer reclassificacao de um voo\n");
    printf("8. Ver historico de pousos\n");
    printf("0. Sair\n");
}

int main(void)
{
    SistemaControle sistema;
    char opcao[TAM_LINHA], codigo[TAM_LINHA], prioridade[TAM_LINHA];
    Voo *voo;
    int id;

    sistema_iniciar(&sistema);

    for (;;) {
        exibir_menu();
        if (!ler_linha("Escolha uma opcao: ", opcao, sizeof opcao))
            break;

        switch (opcao[0]) {
        case '1':
            ler_linha("Codigo do voo (ex.: AZU4521): ", codigo, sizeof codigo);
            ler_prioridade(prioridade, sizeof prioridade);
            voo = sistema_cadastrar_voo(&sistema, codigo, prioridade);
            if (voo == NULL) {
                printf("Erro: codigo ou prioridade invalidos.\n");
            } else {
                printf("Voo registrado: ");
                voo_imprimir(voo);
            }
            break;

        case '2':
            voo = sistema_autorizar_pouso(&sistema);
            if (voo == NULL) {
                printf("Nao ha voos aguardando.\n");
            } else {
                printf("Pouso autorizado: ");
                voo_imprimir(voo);
            }
            break;

        case '3': {
            int contagem[NUM_PRIORIDADES];
            sistema_voos_por_nivel(&sistema, contagem);
            for (int i = 0; i < NUM_PRIORIDADES; i++)
                printf("  %-12s %-24s %d\n", TABELA_PRIORIDADES[i].nome,
                       TABELA_PRIORIDADES[i].descricao, contagem[i]);
            printf("Total aguardando: %d\n", sistema_voos_aguardando(&sistema));
            voo = sistema_proximo_pouso(&sistema);
            if (voo != NULL) {
                printf("Proximo a pousar: ");
                voo_imprimir(voo);
            }
            break;
        }

        case '4':
            id = ler_id();
            if (sistema_posicao_na_fila(&sistema, id) == -1)
                printf("Voo nao encontrado na fila de espera.\n");
            else
                printf("Posicao estimada na sequencia de pouso: %d (0 = proximo)\n",
                       sistema_posicao_na_fila(&sistema, id));
            break;

        case '5':
            id = ler_id();
            ler_prioridade(prioridade, sizeof prioridade);
            printf(sistema_reclassificar_voo(&sistema, id, prioridade)
                       ? "Reclassificacao aplicada.\n"
                       : "Erro: voo nao esta em espera ou prioridade invalida.\n");
            break;

        case '6':
            id = ler_id();
            printf(sistema_desfazer_reclassificacao(&sistema, id)
                       ? "Ultima reclassificacao desfeita.\n"
                       : "Nada para desfazer nesse voo.\n");
            break;

        case '7':
            id = ler_id();
            printf(sistema_refazer_reclassificacao(&sistema, id)
                       ? "Reclassificacao refeita.\n"
                       : "Nada para refazer nesse voo.\n");
            break;

        case '8': {
            static Voo *historico[MAX_HISTORICO];
            int n = sistema_historico(&sistema, historico, MAX_HISTORICO);
            if (n == 0)
                printf("Nenhum pouso realizado ainda.\n");
            for (int i = 0; i < n; i++) {
                printf("  ");
                voo_imprimir(historico[i]);
            }
            break;
        }

        case '0':
            printf("Encerrando o sistema.\n");
            sistema_destruir(&sistema);
            return 0;

        default:
            printf("Opcao invalida.\n");
        }
    }

    sistema_destruir(&sistema);
    return 0;
}
