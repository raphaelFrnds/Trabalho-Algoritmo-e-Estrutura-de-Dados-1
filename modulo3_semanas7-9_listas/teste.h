/*
 * teste.h
 * Mini "framework" de testes, sem bibliotecas externas.
 * Cada checagem imprime [OK] ou [FALHA]; teste_resumo() mostra o total
 * e devolve 1 se algo falhou (para usar como retorno do main).
 */
#ifndef TESTE_H
#define TESTE_H

#include <stdio.h>
#include <string.h>

static int testes_ok = 0;
static int testes_falha = 0;

static inline void checar_int(const char *descricao, long long obtido, long long esperado)
{
    if (obtido == esperado) {
        testes_ok++;
        printf("[OK]    %s\n", descricao);
    } else {
        testes_falha++;
        printf("[FALHA] %s -> esperado=%lld, obtido=%lld\n", descricao, esperado, obtido);
    }
}

static inline void checar_str(const char *descricao, const char *obtido, const char *esperado)
{
    if (obtido != NULL && strcmp(obtido, esperado) == 0) {
        testes_ok++;
        printf("[OK]    %s\n", descricao);
    } else {
        testes_falha++;
        printf("[FALHA] %s -> esperado=\"%s\", obtido=\"%s\"\n", descricao,
               esperado, obtido ? obtido : "(NULL)");
    }
}

static inline int teste_resumo(void)
{
    printf("\nResultado: %d OK, %d falha(s)\n", testes_ok, testes_falha);
    return testes_falha > 0 ? 1 : 0;
}

#endif
