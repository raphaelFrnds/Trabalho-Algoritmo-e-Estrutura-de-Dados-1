/*
 * sistema_controle.h
 * Modulo 6 - Projeto Final (Semanas 14-16).
 *
 * Sistema de Controle de Pousos completo, integrando estruturas
 * estudadas na disciplina:
 *   1. FILA DE PRIORIDADE (Modulo 5) - voos aguardando pouso.
 *   2. LISTA ENCADEADA (Modulo 3) - historico de pousos do dia.
 *   3. PILHA (Modulo 4) - desfazer/refazer reclassificacao feita por
 *      engano ANTES do pouso (extra).
 *
 * Dono da memoria: o sistema aloca cada Voo com malloc no cadastro e
 * libera tudo em sistema_destruir(). As estruturas so guardam ponteiros.
 */
#ifndef SISTEMA_CONTROLE_H
#define SISTEMA_CONTROLE_H

#include <stdbool.h>
#include "voo.h"
#include "fila_prioridade.h"
#include "lista_encadeada_simples.h"
#include "undo_redo_reclassificacao.h"

/*
 * Registro de um voo em espera: o voo + o seu historico de undo/redo.
 * Os registros formam uma lista na ORDEM DE CHEGADA, usada para achar
 * um voo pelo id e para reconstruir a fila apos uma reclassificacao.
 */
typedef struct RegistroEspera {
    Voo *voo;
    GerenciadorReclassificacao gerenciador;
    struct RegistroEspera *proximo;
} RegistroEspera;

typedef struct {
    FilaPrioridade fila_espera;
    ListaSimples historico;            /* mais recente no inicio */
    RegistroEspera *espera_inicio;     /* ordem de chegada */
    RegistroEspera *espera_fim;
} SistemaControle;

void sistema_iniciar(SistemaControle *s);
void sistema_destruir(SistemaControle *s);

/* ---------- Cadastro ---------- */
/* Valida, cria o voo e coloca na fila. NULL se codigo/prioridade forem invalidos. */
Voo *sistema_cadastrar_voo(SistemaControle *s, const char *codigo, const char *prioridade);

/* ---------- Correcao de prioridade (so para voos em espera) ---------- */
bool sistema_reclassificar_voo(SistemaControle *s, int id, const char *nova_prioridade);
bool sistema_desfazer_reclassificacao(SistemaControle *s, int id);
bool sistema_refazer_reclassificacao(SistemaControle *s, int id);

/* ---------- Pouso ---------- */
/* Tira o proximo voo da fila e move para o historico. NULL se nao houver voos. */
Voo *sistema_autorizar_pouso(SistemaControle *s);

/* ---------- Consultas ---------- */
int  sistema_voos_aguardando(const SistemaControle *s);
void sistema_voos_por_nivel(const SistemaControle *s, int contagem[NUM_PRIORIDADES]);
Voo *sistema_proximo_pouso(const SistemaControle *s);
int  sistema_posicao_na_fila(const SistemaControle *s, int id);   /* -1 se nao estiver */
int  sistema_historico(const SistemaControle *s, Voo *destino[], int max);
Voo *sistema_buscar_no_historico(const SistemaControle *s, int id);

#endif
