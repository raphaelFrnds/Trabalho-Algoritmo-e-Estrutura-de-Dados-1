/*
 * test_modulo5.c
 * Testes da fila circular, da fila encadeada e da fila de prioridade.
 */
#include "teste.h"
#include "voo.h"
#include "fila_array_circular.h"
#include "fila_encadeada.h"
#include "fila_prioridade.h"

static Voo voos[10];

static void testar_fila_circular(void)
{
    FilaCircular f;
    Voo *saida = NULL;

    checar_int("circular: capacidade 0 e rejeitada", fila_circ_criar(&f, 0), 0);
    fila_circ_criar(&f, 3);
    checar_int("circular: comeca vazia", fila_circ_esta_vazia(&f), 1);
    checar_int("circular: frente de vazia e NULL", fila_circ_frente(&f) == NULL, 1);
    fila_circ_enfileirar(&f, &voos[0]);
    fila_circ_enfileirar(&f, &voos[1]);
    fila_circ_enfileirar(&f, &voos[2]);
    checar_int("circular: cheia com 3", fila_circ_esta_cheia(&f), 1);
    checar_int("circular: enfileirar cheia falha", fila_circ_enfileirar(&f, &voos[3]), 0);
    fila_circ_desenfileirar(&f, &saida);
    checar_int("circular: FIFO - sai o voo 1", saida->id, 1);
    checar_int("circular: reaproveita a posicao liberada", fila_circ_enfileirar(&f, &voos[3]), 1);
    checar_int("circular: o novo item deu a volta (indice 0)", f.itens[0]->id, 4);
    fila_circ_desenfileirar(&f, &saida);
    fila_circ_desenfileirar(&f, &saida);
    fila_circ_desenfileirar(&f, &saida);
    checar_int("circular: ordem mantida apos dar a volta (ultimo = 4)", saida->id, 4);
    checar_int("circular: desenfileirar vazia falha", fila_circ_desenfileirar(&f, &saida), 0);
    fila_circ_destruir(&f);
}

static void testar_fila_encadeada(void)
{
    FilaEncadeada f;
    Voo *saida = NULL;

    fila_enc_criar(&f);
    checar_int("encadeada: desenfileirar vazia falha", fila_enc_desenfileirar(&f, &saida), 0);
    for (int i = 0; i < 10; i++)
        fila_enc_enfileirar(&f, &voos[i]);
    checar_int("encadeada: tamanho 10", fila_enc_tamanho(&f), 10);
    checar_int("encadeada: frente e o voo 1", fila_enc_frente(&f)->id, 1);
    checar_int("encadeada: posicao do voo 7 e 6", fila_enc_posicao(&f, 7), 6);
    fila_enc_desenfileirar(&f, &saida);
    fila_enc_desenfileirar(&f, &saida);
    checar_int("encadeada: FIFO - segundo a sair e o voo 2", saida->id, 2);
    fila_enc_destruir(&f);
    checar_int("encadeada: vazia apos destruir", fila_enc_esta_vazia(&f), 1);
    fila_enc_enfileirar(&f, &voos[0]);
    checar_int("encadeada: reutilizavel apos destruir", fila_enc_frente(&f)->id, 1);
    fila_enc_destruir(&f);
}

static void testar_fila_prioridade(void)
{
    FilaPrioridade fp;
    Voo *saida = NULL;
    int contagem[NUM_PRIORIDADES];

    Voo a = voo_criar(1, "AZU1", PRIO_REGULAR, "-");
    Voo b = voo_criar(2, "GLO2", PRIO_COMBUSTIVEL, "-");
    Voo c = voo_criar(3, "PTB3", PRIO_TREINAMENTO, "-");
    Voo d = voo_criar(4, "GLO4", PRIO_COMBUSTIVEL, "-");
    Voo e = voo_criar(5, "TAM5", PRIO_MAYDAY, "-");     /* chega por ultimo */
    Voo invalido = voo_criar(6, "X6", (Prioridade)9, "-");

    fila_prio_criar(&fp);
    checar_int("prioridade: vazia no inicio", fila_prio_esta_vazia(&fp), 1);
    checar_int("prioridade: proximo de vazia e NULL", fila_prio_proximo(&fp) == NULL, 1);
    fila_prio_enfileirar(&fp, &a);
    fila_prio_enfileirar(&fp, &b);
    fila_prio_enfileirar(&fp, &c);
    fila_prio_enfileirar(&fp, &d);
    fila_prio_enfileirar(&fp, &e);
    checar_int("prioridade: rejeita nivel invalido", fila_prio_enfileirar(&fp, &invalido), 0);
    checar_int("prioridade: 5 voos aguardando", fila_prio_tamanho(&fp), 5);

    fila_prio_tamanho_por_nivel(&fp, contagem);
    checar_int("prioridade: 2 voos com combustivel minimo", contagem[PRIO_COMBUSTIVEL - 1], 2);

    checar_int("prioridade: posicao do mayday e 0", fila_prio_posicao_estimada(&fp, 5), 0);
    checar_int("prioridade: posicao do 2o combustivel (GLO4) e 2", fila_prio_posicao_estimada(&fp, 4), 2);
    checar_int("prioridade: posicao do treinamento e 4 (ultimo)", fila_prio_posicao_estimada(&fp, 3), 4);
    checar_int("prioridade: id inexistente -> -1", fila_prio_posicao_estimada(&fp, 99), -1);
    checar_str("prioridade: proximo e o mayday", fila_prio_proximo(&fp)->codigo, "TAM5");

    fila_prio_desenfileirar(&fp, &saida);
    checar_str("mayday pousa primeiro, mesmo tendo chegado por ultimo", saida->codigo, "TAM5");
    fila_prio_desenfileirar(&fp, &saida);
    checar_str("FIFO no mesmo nivel: GLO2 (chegou antes) antes de GLO4", saida->codigo, "GLO2");
    fila_prio_desenfileirar(&fp, &saida);
    checar_str("depois GLO4", saida->codigo, "GLO4");
    fila_prio_desenfileirar(&fp, &saida);
    checar_str("regular antes de treinamento", saida->codigo, "AZU1");
    fila_prio_desenfileirar(&fp, &saida);
    checar_str("treinamento por ultimo", saida->codigo, "PTB3");
    checar_int("desenfileirar vazia falha", fila_prio_desenfileirar(&fp, &saida), 0);
    fila_prio_destruir(&fp);
}

int main(void)
{
    for (int i = 0; i < 10; i++)
        voos[i] = voo_criar(i + 1, "VOO", PRIO_REGULAR, "-");

    testar_fila_circular();
    testar_fila_encadeada();
    testar_fila_prioridade();
    return teste_resumo();
}
