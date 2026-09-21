/*
 * test_modulo1.c
 * Testes do Entregavel 1: entradas e saidas esperadas de cada funcao.
 * Compilar e rodar:  make test
 */
#include "teste.h"
#include "voo.h"
#include "utils.h"
#include "validacao.h"
#include "parametros_demo.h"
#include "recursao_demo.h"

static void testar_utils(void)
{
    char hora[TAM_HORA];
    struct tm momento = {0};

    resetar_contador_id();
    checar_int("primeiro id gerado deve ser 1", gerar_id_voo(), 1);
    checar_int("segundo id gerado deve ser 2", gerar_id_voo(), 2);
    checar_int("terceiro id gerado deve ser 3", gerar_id_voo(), 3);

    momento.tm_mday = 25; momento.tm_mon = 7; momento.tm_year = 2026 - 1900;
    momento.tm_hour = 14; momento.tm_min = 30; momento.tm_sec = 0;
    formatar_hora_chegada(hora, sizeof hora, &momento);
    checar_str("formatacao de data/hora fixa", hora, "25/08/2026 14:30:00");
}

static void testar_validacao(void)
{
    checar_int("'mayday' e prioridade valida", validar_prioridade("mayday"), 1);
    checar_int("'  REGULAR ' e valida (ignora caixa e espacos)", validar_prioridade("  REGULAR "), 1);
    checar_int("'urgente' nao e valida", validar_prioridade("urgente"), 0);
    checar_int("string vazia nao e valida", validar_prioridade(""), 0);
    checar_int("NULL nao e valida", validar_prioridade(NULL), 0);
    checar_int("codigo 'AZU4521' valido", validar_codigo("AZU4521"), 1);
    checar_int("codigo vazio invalido", validar_codigo("   "), 0);
    checar_int("codigo longo demais invalido", validar_codigo("CODIGOMUITOLONGO123"), 0);
    checar_int("mayday -> 1", prioridade_numerica("mayday"), 1);
    checar_int("combustivel -> 3", prioridade_numerica("combustivel"), 3);
    checar_int("treinamento -> 5", prioridade_numerica("treinamento"), 5);
    checar_int("invalida -> -1", prioridade_numerica("xyz"), -1);
    checar_str("nome do nivel 2", nome_prioridade(PRIO_PANPAN), "panpan");
}

static void testar_parametros(void)
{
    Voo v = voo_criar(1, "AZU4521", PRIO_REGULAR, "25/08/2026 10:00:00");
    int a = 10, b = 20;

    checar_int("copia recebe a nova prioridade",
               tentar_reclassificar_copia(v, PRIO_MAYDAY), PRIO_MAYDAY);
    checar_int("por valor: original continua regular", v.prioridade, PRIO_REGULAR);

    reclassificar_voo(&v, PRIO_PANPAN);
    checar_int("por referencia: original virou panpan", v.prioridade, PRIO_PANPAN);

    trocar_por_valor(a, b);
    checar_int("trocar_por_valor nao altera a", a, 10);
    trocar_valores(&a, &b);
    checar_int("trocar_valores: a=20", a, 20);
    checar_int("trocar_valores: b=10", b, 10);
}

static void testar_recursao(void)
{
    Voo voos[4] = {
        voo_criar(1, "A1", PRIO_REGULAR, "-"),
        voo_criar(3, "B3", PRIO_PANPAN, "-"),
        voo_criar(7, "C7", PRIO_MAYDAY, "-"),
        voo_criar(9, "D9", PRIO_TREINAMENTO, "-"),
    };

    checar_int("0! = 1", fatorial(0), 1);
    checar_int("5! = 120", fatorial(5), 120);
    checar_int("fatorial negativo -> -1", fatorial(-3), -1);
    checar_int("fib(0) = 0", fibonacci(0), 0);
    checar_int("fib(10) = 55", fibonacci(10), 55);
    checar_int("busca id=7 -> indice 2", busca_binaria_recursiva(voos, 7, 0, 3), 2);
    checar_int("busca id=1 (primeiro) -> 0", busca_binaria_recursiva(voos, 1, 0, 3), 0);
    checar_int("busca id=2 (inexistente) -> -1", busca_binaria_recursiva(voos, 2, 0, 3), -1);
}

int main(void)
{
    testar_utils();
    testar_validacao();
    testar_parametros();
    testar_recursao();
    return teste_resumo();
}
