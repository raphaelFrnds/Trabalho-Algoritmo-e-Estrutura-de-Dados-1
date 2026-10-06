/*
 * utils.c
 */
#include "utils.h"

/* 'static' fora de funcao: a variavel so e visivel neste arquivo,
 * entao ninguem de fora consegue baguncar o contador. */
static int proximo_id = 1;

int gerar_id_voo(void)
{
    return proximo_id++;
}

void resetar_contador_id(void)
{
    proximo_id = 1;
}

void formatar_hora_chegada(char *destino, size_t tam, const struct tm *momento)
{
    if (momento == NULL) {
        time_t agora = time(NULL);
        momento = localtime(&agora);
    }
    strftime(destino, tam, "%d/%m/%Y %H:%M:%S", momento);
}
