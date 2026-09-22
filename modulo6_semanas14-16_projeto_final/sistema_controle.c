/*
 * sistema_controle.c
 */
#include <stdlib.h>
#include "sistema_controle.h"
#include "utils.h"
#include "validacao.h"

void sistema_iniciar(SistemaControle *s)
{
    fila_prio_criar(&s->fila_espera);
    lista_simples_criar(&s->historico);
    s->espera_inicio = s->espera_fim = NULL;
}

/* Procura o registro de um voo em espera. O(n). */
static RegistroEspera *buscar_registro(const SistemaControle *s, int id)
{
    for (RegistroEspera *r = s->espera_inicio; r != NULL; r = r->proximo)
        if (r->voo->id == id)
            return r;
    return NULL;
}

/* Tira o registro da lista de espera e libera SO o registro (o voo segue vivo). */
static void remover_registro(SistemaControle *s, int id)
{
    RegistroEspera *anterior = NULL, *atual = s->espera_inicio;

    while (atual != NULL && atual->voo->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL)
        return;

    if (anterior == NULL)
        s->espera_inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;
    if (atual == s->espera_fim)
        s->espera_fim = anterior;
    free(atual);
}

/*
 * A fila de prioridade separa os voos por nivel. Quando um voo muda de
 * nivel, ele precisa "trocar de fila" - e uma fila FIFO nao remove do
 * meio. Solucao: refazer a fila a partir dos voos em espera, na ordem
 * de chegada. O(n), aceitavel porque reclassificar e raro.
 */
static void reconstruir_fila(SistemaControle *s)
{
    fila_prio_destruir(&s->fila_espera);
    fila_prio_criar(&s->fila_espera);
    for (RegistroEspera *r = s->espera_inicio; r != NULL; r = r->proximo)
        fila_prio_enfileirar(&s->fila_espera, r->voo);
}

Voo *sistema_cadastrar_voo(SistemaControle *s, const char *codigo, const char *prioridade)
{
    char hora[TAM_HORA];

    if (!validar_codigo(codigo) || !validar_prioridade(prioridade))
        return NULL;

    Voo *voo = malloc(sizeof(Voo));
    RegistroEspera *registro = malloc(sizeof(RegistroEspera));
    if (voo == NULL || registro == NULL) {
        free(voo);                 /* free(NULL) e permitido e nao faz nada */
        free(registro);
        return NULL;
    }

    formatar_hora_chegada(hora, sizeof hora, NULL);
    *voo = voo_criar(gerar_id_voo(), codigo, (Prioridade)prioridade_numerica(prioridade), hora);

    if (!fila_prio_enfileirar(&s->fila_espera, voo)) {
        free(voo);
        free(registro);
        return NULL;
    }

    registro->voo = voo;
    gerenciador_iniciar(&registro->gerenciador, voo);
    registro->proximo = NULL;
    if (s->espera_fim == NULL)
        s->espera_inicio = registro;
    else
        s->espera_fim->proximo = registro;
    s->espera_fim = registro;
    return voo;
}

bool sistema_reclassificar_voo(SistemaControle *s, int id, const char *nova_prioridade)
{
    RegistroEspera *r = buscar_registro(s, id);

    if (r == NULL || !validar_prioridade(nova_prioridade))
        return false;
    if (!gerenciador_reclassificar(&r->gerenciador,
                                   (Prioridade)prioridade_numerica(nova_prioridade)))
        return false;
    reconstruir_fila(s);
    return true;
}

bool sistema_desfazer_reclassificacao(SistemaControle *s, int id)
{
    RegistroEspera *r = buscar_registro(s, id);

    if (r == NULL || !gerenciador_desfazer(&r->gerenciador))
        return false;
    reconstruir_fila(s);
    return true;
}

bool sistema_refazer_reclassificacao(SistemaControle *s, int id)
{
    RegistroEspera *r = buscar_registro(s, id);

    if (r == NULL || !gerenciador_refazer(&r->gerenciador))
        return false;
    reconstruir_fila(s);
    return true;
}

Voo *sistema_autorizar_pouso(SistemaControle *s)
{
    Voo *voo;

    if (!fila_prio_desenfileirar(&s->fila_espera, &voo))
        return NULL;
    remover_registro(s, voo->id);            /* sai da espera (e perde o undo/redo) */

    /* Insere no INICIO: O(1) e o historico fica do mais recente ao mais antigo. */
    lista_simples_inserir(&s->historico, voo, 0);
    return voo;
}

int sistema_voos_aguardando(const SistemaControle *s)
{
    return fila_prio_tamanho(&s->fila_espera);
}

void sistema_voos_por_nivel(const SistemaControle *s, int contagem[NUM_PRIORIDADES])
{
    fila_prio_tamanho_por_nivel(&s->fila_espera, contagem);
}

Voo *sistema_proximo_pouso(const SistemaControle *s)
{
    return fila_prio_proximo(&s->fila_espera);
}

int sistema_posicao_na_fila(const SistemaControle *s, int id)
{
    return fila_prio_posicao_estimada(&s->fila_espera, id);
}

int sistema_historico(const SistemaControle *s, Voo *destino[], int max)
{
    return lista_simples_percorrer(&s->historico, destino, max);
}

Voo *sistema_buscar_no_historico(const SistemaControle *s, int id)
{
    return lista_simples_obter(&s->historico, lista_simples_buscar(&s->historico, id));
}

void sistema_destruir(SistemaControle *s)
{
    Voo *voo;

    /* 1. voos em espera: libera o voo e o registro */
    RegistroEspera *r = s->espera_inicio;
    while (r != NULL) {
        RegistroEspera *proximo = r->proximo;
        free(r->voo);
        free(r);
        r = proximo;
    }
    s->espera_inicio = s->espera_fim = NULL;

    /* 2. nos da fila (os voos ja foram liberados acima) */
    fila_prio_destruir(&s->fila_espera);

    /* 3. historico: libera cada voo pousado e o no da lista */
    while (lista_simples_remover(&s->historico, 0, &voo))
        free(voo);
}
