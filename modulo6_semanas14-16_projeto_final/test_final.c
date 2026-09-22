/*
 * test_final.c
 * Testes de INTEGRACAO do projeto final: fila de prioridade +
 * historico (lista encadeada) + desfazer/refazer (pilha).
 */
#include "teste.h"
#include "sistema_controle.h"
#include "utils.h"

static void testar_fluxo_completo(void)
{
    SistemaControle s;
    Voo *historico[10];

    sistema_iniciar(&s);
    sistema_cadastrar_voo(&s, "AZU4521", "regular");
    sistema_cadastrar_voo(&s, "GLO1180", "combustivel");
    sistema_cadastrar_voo(&s, "PTB2210", "treinamento");

    checar_int("3 voos aguardando apos cadastro", sistema_voos_aguardando(&s), 3);
    checar_str("combustivel pousa primeiro", sistema_autorizar_pouso(&s)->codigo, "GLO1180");
    checar_int("historico com 1 voo", sistema_historico(&s, historico, 10), 1);
    checar_str("regular antes de treinamento", sistema_autorizar_pouso(&s)->codigo, "AZU4521");
    checar_str("treinamento por ultimo", sistema_autorizar_pouso(&s)->codigo, "PTB2210");
    checar_int("fila vazia ao final", sistema_voos_aguardando(&s), 0);
    checar_int("historico completo com 3 voos", sistema_historico(&s, historico, 10), 3);
    checar_str("historico comeca pelo mais recente", historico[0]->codigo, "PTB2210");
    sistema_destruir(&s);
}

static void testar_cadastro_invalido(void)
{
    SistemaControle s;

    sistema_iniciar(&s);
    checar_int("prioridade invalida e rejeitada", sistema_cadastrar_voo(&s, "AZU1", "urgente") == NULL, 1);
    checar_int("codigo vazio e rejeitado", sistema_cadastrar_voo(&s, "", "regular") == NULL, 1);
    checar_int("nada entrou na fila", sistema_voos_aguardando(&s), 0);
    checar_int("pouso com fila vazia devolve NULL", sistema_autorizar_pouso(&s) == NULL, 1);
    sistema_destruir(&s);
}

static void testar_reclassificacao(void)
{
    SistemaControle s;

    sistema_iniciar(&s);
    Voo *a = sistema_cadastrar_voo(&s, "PTB2210", "treinamento");
    Voo *b = sistema_cadastrar_voo(&s, "AZU4521", "regular");
    (void)b;

    checar_int("antes: treinamento esta em ultimo (posicao 1)", sistema_posicao_na_fila(&s, a->id), 1);

    /* O piloto declara emergencia: PTB2210 vira mayday. */
    sistema_reclassificar_voo(&s, a->id, "mayday");
    checar_int("apos reclassificar: posicao 0", sistema_posicao_na_fila(&s, a->id), 0);
    checar_str("proximo pouso agora e o PTB2210", sistema_proximo_pouso(&s)->codigo, "PTB2210");

    sistema_desfazer_reclassificacao(&s, a->id);
    checar_int("desfazer: volta para treinamento", a->prioridade, PRIO_TREINAMENTO);
    checar_str("desfazer: ordem original restaurada", sistema_proximo_pouso(&s)->codigo, "AZU4521");

    sistema_refazer_reclassificacao(&s, a->id);
    checar_str("refazer: PTB2210 volta a ser o proximo", sistema_proximo_pouso(&s)->codigo, "PTB2210");

    checar_int("reclassificar id inexistente falha", sistema_reclassificar_voo(&s, 999, "mayday"), 0);
    checar_int("reclassificar com prioridade invalida falha", sistema_reclassificar_voo(&s, a->id, "xyz"), 0);

    sistema_autorizar_pouso(&s);                          /* PTB2210 pousa */
    checar_int("voo que ja pousou nao pode ser reclassificado",
               sistema_reclassificar_voo(&s, a->id, "regular"), 0);
    checar_int("voo que ja pousou nao tem desfazer", sistema_desfazer_reclassificacao(&s, a->id), 0);
    sistema_destruir(&s);
}

static void testar_fifo_apos_reclassificacao(void)
{
    SistemaControle s;

    sistema_iniciar(&s);
    Voo *a = sistema_cadastrar_voo(&s, "AAA1", "regular");
    sistema_cadastrar_voo(&s, "BBB2", "combustivel");
    sistema_cadastrar_voo(&s, "CCC3", "combustivel");

    /* AAA1 chegou primeiro; ao virar combustivel, deve ficar a frente dos outros dois. */
    sistema_reclassificar_voo(&s, a->id, "combustivel");
    checar_str("reconstrucao respeita a ordem de chegada (AAA1)", sistema_autorizar_pouso(&s)->codigo, "AAA1");
    checar_str("depois BBB2", sistema_autorizar_pouso(&s)->codigo, "BBB2");
    sistema_destruir(&s);
}

static void testar_busca_historico(void)
{
    SistemaControle s;

    sistema_iniciar(&s);
    Voo *a = sistema_cadastrar_voo(&s, "GLO1555", "mayday");
    sistema_cadastrar_voo(&s, "TAM3090", "regular");
    sistema_autorizar_pouso(&s);

    checar_str("busca no historico encontra GLO1555", sistema_buscar_no_historico(&s, a->id)->codigo, "GLO1555");
    checar_int("voo ainda em espera nao esta no historico", sistema_buscar_no_historico(&s, a->id + 1) == NULL, 1);

    int contagem[NUM_PRIORIDADES];
    sistema_voos_por_nivel(&s, contagem);
    checar_int("contagem por nivel: 1 regular aguardando", contagem[PRIO_REGULAR - 1], 1);
    sistema_destruir(&s);
}

int main(void)
{
    resetar_contador_id();
    testar_fluxo_completo();
    testar_cadastro_invalido();
    testar_reclassificacao();
    testar_fifo_apos_reclassificacao();
    testar_busca_historico();
    return teste_resumo();
}
