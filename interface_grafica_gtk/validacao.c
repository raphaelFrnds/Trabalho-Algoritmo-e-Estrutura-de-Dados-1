/*
 * validacao.c
 * Implementacao da validacao de entrada.
 */
#include <ctype.h>
#include <string.h>
#include "validacao.h"

const InfoPrioridade TABELA_PRIORIDADES[NUM_PRIORIDADES] = {
    {"mayday",      "Emergencia (MAYDAY)",     PRIO_MAYDAY,      0},
    {"panpan",      "Urgencia (PAN-PAN)",      PRIO_PANPAN,      5},
    {"combustivel", "Combustivel minimo",      PRIO_COMBUSTIVEL, 10},
    {"regular",     "Voo regular",             PRIO_REGULAR,     30},
    {"treinamento", "Treinamento / translado", PRIO_TREINAMENTO, 60},
};

/*
 * Copia 'origem' para 'destino' sem espacos nas pontas e em minusculas.
 * Funcao auxiliar (static = visivel so dentro deste arquivo).
 */
static void normalizar(const char *origem, char *destino, size_t tam)
{
    size_t ini = 0, fim = strlen(origem), n = 0;

    while (ini < fim && isspace((unsigned char)origem[ini])) ini++;
    while (fim > ini && isspace((unsigned char)origem[fim - 1])) fim--;

    for (size_t i = ini; i < fim && n + 1 < tam; i++)
        destino[n++] = (char)tolower((unsigned char)origem[i]);
    destino[n] = '\0';
}

int prioridade_numerica(const char *nome)
{
    char texto[32];

    if (nome == NULL)
        return -1;
    normalizar(nome, texto, sizeof texto);

    for (int i = 0; i < NUM_PRIORIDADES; i++)
        if (strcmp(texto, TABELA_PRIORIDADES[i].nome) == 0)
            return (int)TABELA_PRIORIDADES[i].nivel;
    return -1;
}

bool validar_prioridade(const char *nome)
{
    return prioridade_numerica(nome) != -1;
}

bool validar_codigo(const char *codigo)
{
    char texto[64];

    if (codigo == NULL || strlen(codigo) >= sizeof texto)
        return false;
    normalizar(codigo, texto, sizeof texto);
    return strlen(texto) > 0 && strlen(texto) < TAM_CODIGO;
}

const char *nome_prioridade(Prioridade nivel)
{
    if (nivel < 1 || nivel > NUM_PRIORIDADES)
        return "invalida";
    return TABELA_PRIORIDADES[nivel - 1].nome;
}
