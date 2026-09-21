/*
 * recursao_demo.h
 * Semana 3 - Recursividade: fatorial, Fibonacci e busca binaria
 * recursiva (ja adaptada para buscar um Voo pelo ID).
 */
#ifndef RECURSAO_DEMO_H
#define RECURSAO_DEMO_H

#include "voo.h"

/* n! recursivo. Retorna -1 se n for negativo. */
long long fatorial(int n);

/* n-esimo termo de Fibonacci (fib(0)=0, fib(1)=1). Retorna -1 se n < 0. */
long long fibonacci(int n);

/*
 * Busca o voo com 'id' no vetor 'voos', ORDENADO por id crescente,
 * entre as posicoes 'inicio' e 'fim' (inclusive).
 * Retorna o indice encontrado ou -1. Complexidade: O(log n).
 */
int busca_binaria_recursiva(const Voo voos[], int id, int inicio, int fim);

#endif
