/*
 * test_modulo4.c
 * Testes das duas pilhas e do desfazer/refazer de reclassificacao.
 */
#include "teste.h"
#include "voo.h"
#include "pilha_array.h"
#include "pilha_encadeada.h"
#include "undo_redo_reclassificacao.h"

static void testar_pilha_array(void)
{
    PilhaArray p;
    Prioridade x = 0;

    pilha_array_criar(&p);
    checar_int("array: comeca vazia", pilha_array_esta_vazia(&p), 1);
    checar_int("array: desempilhar vazia falha", pilha_array_desempilhar(&p, &x), 0);
    checar_int("array: topo de vazia falha", pilha_array_topo(&p, &x), 0);
    pilha_array_empilhar(&p, PRIO_REGULAR);
    pilha_array_empilhar(&p, PRIO_PANPAN);
    pilha_array_empilhar(&p, PRIO_MAYDAY);
    pilha_array_topo(&p, &x);
    checar_int("array: topo e o ultimo empilhado (mayday)", x, PRIO_MAYDAY);
    checar_int("array: tamanho 3", pilha_array_tamanho(&p), 3);
    pilha_array_desempilhar(&p, &x);
    checar_int("array: LIFO - sai mayday", x, PRIO_MAYDAY);
    pilha_array_desempilhar(&p, &x);
    checar_int("array: LIFO - depois panpan", x, PRIO_PANPAN);
    for (int i = 0; i < PILHA_CAP; i++)
        pilha_array_empilhar(&p, PRIO_REGULAR);
    checar_int("array: fica cheia", pilha_array_esta_cheia(&p), 1);
    checar_int("array: empilhar cheia falha", pilha_array_empilhar(&p, PRIO_REGULAR), 0);
}

static void testar_pilha_encadeada(void)
{
    PilhaEncadeada p;
    Prioridade x = 0;

    pilha_enc_criar(&p);
    checar_int("encadeada: desempilhar vazia falha", pilha_enc_desempilhar(&p, &x), 0);
    for (int i = 0; i < 200; i++)                     /* passa do limite do vetor */
        pilha_enc_empilhar(&p, (i % 2) ? PRIO_PANPAN : PRIO_REGULAR);
    checar_int("encadeada: aceita 200 itens (sem limite)", pilha_enc_tamanho(&p), 200);
    pilha_enc_empilhar(&p, PRIO_COMBUSTIVEL);
    pilha_enc_topo(&p, &x);
    checar_int("encadeada: topo e o ultimo (combustivel)", x, PRIO_COMBUSTIVEL);
    pilha_enc_desempilhar(&p, &x);
    pilha_enc_desempilhar(&p, &x);
    checar_int("encadeada: LIFO - o 200o item era panpan", x, PRIO_PANPAN);
    pilha_enc_destruir(&p);
    checar_int("encadeada: vazia apos destruir", pilha_enc_esta_vazia(&p), 1);
}

static void testar_undo_redo(void)
{
    Voo voo = voo_criar(1, "GLO1180", PRIO_REGULAR, "-");
    GerenciadorReclassificacao g;

    gerenciador_iniciar(&g, &voo);
    checar_int("undo/redo: nada para desfazer no inicio", gerenciador_desfazer(&g), 0);
    checar_int("undo/redo: nada para refazer no inicio", gerenciador_refazer(&g), 0);

    gerenciador_reclassificar(&g, PRIO_COMBUSTIVEL);
    gerenciador_reclassificar(&g, PRIO_PANPAN);
    checar_int("reclassificar altera o voo original (ponteiro)", voo.prioridade, PRIO_PANPAN);

    gerenciador_desfazer(&g);
    checar_int("desfazer 1x -> combustivel", voo.prioridade, PRIO_COMBUSTIVEL);
    gerenciador_desfazer(&g);
    checar_int("desfazer 2x -> regular (original)", voo.prioridade, PRIO_REGULAR);
    checar_int("nao ha mais o que desfazer", gerenciador_pode_desfazer(&g), 0);

    gerenciador_refazer(&g);
    checar_int("refazer -> combustivel", voo.prioridade, PRIO_COMBUSTIVEL);
    checar_int("ainda da para refazer", gerenciador_pode_refazer(&g), 1);

    gerenciador_reclassificar(&g, PRIO_MAYDAY);
    checar_int("nova reclassificacao descarta o refazer", gerenciador_pode_refazer(&g), 0);
    gerenciador_desfazer(&g);
    checar_int("desfazer volta para combustivel", voo.prioridade, PRIO_COMBUSTIVEL);
}

int main(void)
{
    testar_pilha_array();
    testar_pilha_encadeada();
    testar_undo_redo();
    return teste_resumo();
}
