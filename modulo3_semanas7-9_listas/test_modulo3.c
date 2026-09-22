/*
 * test_modulo3.c
 * Testes das variacoes de lista: vetor, encadeada simples, dupla e circular.
 */
#include <stdio.h>
#include "teste.h"
#include "voo.h"
#include "lista_array.h"
#include "lista_encadeada_simples.h"
#include "lista_dupla_circular.h"

/* Voos de teste: ficam no main, as listas so guardam o endereco deles. */
static Voo v1, v2, v3, v4;

/* Transforma um vetor de voos em texto "1,2,3" para facilitar a checagem. */
static const char *ids(Voo *voos[], int n)
{
    static char texto[128];
    int pos = 0;
    texto[0] = '\0';
    for (int i = 0; i < n; i++)
        pos += snprintf(texto + pos, sizeof texto - pos, i ? ",%d" : "%d", voos[i]->id);
    return texto;
}

static void testar_lista_array(void)
{
    ListaArray l;
    Voo *saida[10], *removido = NULL;

    lista_array_criar(&l);
    checar_int("array: comeca vazia", lista_array_esta_vazia(&l), 1);
    lista_array_inserir(&l, &v1, POSICAO_FIM);
    lista_array_inserir(&l, &v3, POSICAO_FIM);
    lista_array_inserir(&l, &v2, 1);                      /* no meio */
    lista_array_inserir(&l, &v4, 0);                      /* no inicio */
    checar_str("array: ordem apos inserir no meio e no inicio",
               ids(saida, lista_array_percorrer(&l, saida, 10)), "4,1,2,3");
    checar_int("array: posicao invalida e rejeitada", lista_array_inserir(&l, &v1, 9), 0);
    checar_int("array: busca id=2 -> posicao 2", lista_array_buscar(&l, 2), 2);
    checar_int("array: busca id=99 -> -1", lista_array_buscar(&l, 99), -1);
    lista_array_remover(&l, 0, &removido);
    checar_int("array: remove do inicio devolve o voo 4", removido->id, 4);
    checar_str("array: ordem apos remover", ids(saida, lista_array_percorrer(&l, saida, 10)), "1,2,3");
    checar_int("array: remover posicao invalida falha", lista_array_remover(&l, 5, NULL), 0);
}

static void testar_lista_simples(void)
{
    ListaSimples l;
    Voo *saida[10], *removido = NULL;

    lista_simples_criar(&l);
    lista_simples_inserir(&l, &v1, POSICAO_FIM);
    lista_simples_inserir(&l, &v3, POSICAO_FIM);
    lista_simples_inserir(&l, &v2, 1);
    lista_simples_inserir(&l, &v4, 0);
    checar_str("simples: ordem 4,1,2,3", ids(saida, lista_simples_percorrer(&l, saida, 10)), "4,1,2,3");
    checar_int("simples: tamanho 4", lista_simples_tamanho(&l), 4);
    checar_int("simples: busca id=3 -> 3", lista_simples_buscar(&l, 3), 3);
    checar_int("simples: obter posicao 2 -> voo 2", lista_simples_obter(&l, 2)->id, 2);
    checar_int("simples: obter posicao invalida -> NULL", lista_simples_obter(&l, 7) == NULL, 1);
    lista_simples_remover(&l, 3, &removido);             /* remove o ultimo */
    checar_int("simples: removeu o ultimo (id 3)", removido->id, 3);
    lista_simples_inserir(&l, &v3, POSICAO_FIM);         /* 'fim' foi atualizado? */
    checar_str("simples: ponteiro fim continua certo", ids(saida, lista_simples_percorrer(&l, saida, 10)), "4,1,2,3");
    lista_simples_remover(&l, 1, NULL);                  /* remove do meio */
    checar_str("simples: remover do meio", ids(saida, lista_simples_percorrer(&l, saida, 10)), "4,2,3");
    lista_simples_destruir(&l);
    checar_int("simples: vazia apos destruir", lista_simples_esta_vazia(&l), 1);
}

static void testar_lista_dupla(void)
{
    ListaDupla l;
    Voo *saida[10];

    lista_dupla_criar(&l, false);
    lista_dupla_inserir(&l, &v1, POSICAO_FIM);
    lista_dupla_inserir(&l, &v2, POSICAO_FIM);
    lista_dupla_inserir(&l, &v4, POSICAO_FIM);
    lista_dupla_inserir(&l, &v3, 2);
    checar_str("dupla: ordem direta", ids(saida, lista_dupla_percorrer(&l, saida, 10)), "1,2,3,4");
    checar_str("dupla: ordem reversa", ids(saida, lista_dupla_percorrer_reverso(&l, saida, 10)), "4,3,2,1");
    checar_int("dupla (nao circular): proximo do fim e NULL", l.fim->proximo == NULL, 1);
    lista_dupla_remover(&l, 3, NULL);                    /* ultimo */
    lista_dupla_remover(&l, 0, NULL);                    /* primeiro */
    checar_str("dupla: apos remover as pontas", ids(saida, lista_dupla_percorrer(&l, saida, 10)), "2,3");
    checar_str("dupla: reverso apos remover", ids(saida, lista_dupla_percorrer_reverso(&l, saida, 10)), "3,2");
    lista_dupla_destruir(&l);
}

static void testar_lista_circular(void)
{
    ListaDupla l;
    Voo *saida[10];

    lista_dupla_criar(&l, true);
    lista_dupla_inserir(&l, &v1, POSICAO_FIM);
    lista_dupla_inserir(&l, &v2, POSICAO_FIM);
    lista_dupla_inserir(&l, &v3, POSICAO_FIM);
    checar_int("circular: proximo do fim aponta para o inicio", l.fim->proximo == l.inicio, 1);
    checar_int("circular: anterior do inicio aponta para o fim", l.inicio->anterior == l.fim, 1);
    checar_int("circular: dar a volta (3 passos) retorna ao inicio",
               l.inicio->proximo->proximo->proximo == l.inicio, 1);
    checar_str("circular: percorrer para no tamanho (sem loop infinito)",
               ids(saida, lista_dupla_percorrer(&l, saida, 10)), "1,2,3");
    lista_dupla_remover(&l, 0, NULL);
    checar_int("circular: apos remover o inicio, fim->proximo e o novo inicio",
               l.fim->proximo == l.inicio && l.inicio->voo->id == 2, 1);
    lista_dupla_remover(&l, 0, NULL);
    lista_dupla_remover(&l, 0, NULL);
    checar_int("circular: vazia apos remover tudo", lista_dupla_esta_vazia(&l), 1);
    lista_dupla_destruir(&l);
}

int main(void)
{
    v1 = voo_criar(1, "AZU1", PRIO_REGULAR, "-");
    v2 = voo_criar(2, "GLO2", PRIO_PANPAN, "-");
    v3 = voo_criar(3, "TAM3", PRIO_COMBUSTIVEL, "-");
    v4 = voo_criar(4, "PTB4", PRIO_TREINAMENTO, "-");

    testar_lista_array();
    testar_lista_simples();
    testar_lista_dupla();
    testar_lista_circular();
    return teste_resumo();
}
