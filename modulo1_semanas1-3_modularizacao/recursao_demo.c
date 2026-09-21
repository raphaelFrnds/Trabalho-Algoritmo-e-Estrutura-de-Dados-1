/*
 * recursao_demo.c
 */
#include "recursao_demo.h"

long long fatorial(int n)
{
    if (n < 0)
        return -1;                      /* nao definido */
    if (n <= 1)
        return 1;                       /* caso base */
    return n * fatorial(n - 1);         /* passo recursivo */
}

long long fibonacci(int n)
{
    if (n < 0)
        return -1;
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int busca_binaria_recursiva(const Voo voos[], int id, int inicio, int fim)
{
    if (inicio > fim)
        return -1;                      /* espaco de busca vazio */

    int meio = inicio + (fim - inicio) / 2;   /* evita overflow de (inicio+fim) */

    if (voos[meio].id == id)
        return meio;
    if (voos[meio].id < id)
        return busca_binaria_recursiva(voos, id, meio + 1, fim);
    return busca_binaria_recursiva(voos, id, inicio, meio - 1);
}
