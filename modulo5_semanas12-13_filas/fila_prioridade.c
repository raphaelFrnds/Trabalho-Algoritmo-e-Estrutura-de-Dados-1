/*
 * fila_prioridade.c
 */
#include "fila_prioridade.h"

void fila_prio_criar(FilaPrioridade *fp)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        fila_enc_criar(&fp->niveis[i]);
}

bool fila_prio_enfileirar(FilaPrioridade *fp, Voo *voo)
{
    int nivel = (int)voo->prioridade;
    if (nivel < 1 || nivel > NUM_PRIORIDADES)
        return false;
    return fila_enc_enfileirar(&fp->niveis[nivel - 1], voo);
}

/* Olha os niveis do mais urgente (indice 0) para o menos urgente. */
bool fila_prio_desenfileirar(FilaPrioridade *fp, Voo **saida)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        if (!fila_enc_esta_vazia(&fp->niveis[i]))
            return fila_enc_desenfileirar(&fp->niveis[i], saida);
    return false;
}

Voo *fila_prio_proximo(const FilaPrioridade *fp)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        if (!fila_enc_esta_vazia(&fp->niveis[i]))
            return fila_enc_frente(&fp->niveis[i]);
    return NULL;
}

bool fila_prio_esta_vazia(const FilaPrioridade *fp)
{
    return fila_prio_tamanho(fp) == 0;
}

int fila_prio_tamanho(const FilaPrioridade *fp)
{
    int total = 0;
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        total += fila_enc_tamanho(&fp->niveis[i]);
    return total;
}

void fila_prio_tamanho_por_nivel(const FilaPrioridade *fp, int contagem[NUM_PRIORIDADES])
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        contagem[i] = fila_enc_tamanho(&fp->niveis[i]);
}

/* Soma todos os voos dos niveis mais urgentes + a posicao dentro do nivel. */
int fila_prio_posicao_estimada(const FilaPrioridade *fp, int id_voo)
{
    int a_frente = 0;
    for (int i = 0; i < NUM_PRIORIDADES; i++) {
        int pos = fila_enc_posicao(&fp->niveis[i], id_voo);
        if (pos != -1)
            return a_frente + pos;
        a_frente += fila_enc_tamanho(&fp->niveis[i]);
    }
    return -1;
}

void fila_prio_destruir(FilaPrioridade *fp)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        fila_enc_destruir(&fp->niveis[i]);
}
