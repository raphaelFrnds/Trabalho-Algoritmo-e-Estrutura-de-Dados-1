/*
 * test_modulo2.c
 * Testes do Entregavel 2: o mesmo roteiro de testes aplicado as duas
 * implementacoes do TAD (estatica e dinamica).
 */
#include "teste.h"
#include "voo.h"
#include "repositorio_estatico.h"
#include "repositorio_dinamico.h"

static Voo novo_voo(int id, const char *codigo, Prioridade p)
{
    return voo_criar(id, codigo, p, "25/08/2026 10:00:00");
}

static void testar_estatico(void)
{
    RepositorioEstatico repo;     /* variavel local: nada de malloc */
    Voo lista[CAPACIDADE_MAX];

    checar_int("estatico: capacidade 0 e rejeitada", repo_estatico_criar(&repo, 0), 0);
    checar_int("estatico: capacidade acima do maximo e rejeitada",
               repo_estatico_criar(&repo, CAPACIDADE_MAX + 1), 0);
    repo_estatico_criar(&repo, 3);
    checar_int("estatico: comeca vazio", repo_estatico_quantidade(&repo), 0);
    checar_int("estatico: insere voo 1", repo_estatico_inserir(&repo, novo_voo(1, "AZU1", PRIO_REGULAR)), 1);
    checar_int("estatico: insere voo 2", repo_estatico_inserir(&repo, novo_voo(2, "GLO2", PRIO_MAYDAY)), 1);
    checar_int("estatico: insere voo 3", repo_estatico_inserir(&repo, novo_voo(3, "TAM3", PRIO_PANPAN)), 1);
    checar_int("estatico: fica cheio com 3", repo_estatico_esta_cheio(&repo), 1);
    checar_int("estatico: rejeita o 4o voo", repo_estatico_inserir(&repo, novo_voo(4, "X4", PRIO_REGULAR)), 0);
    checar_str("estatico: busca id=2", repo_estatico_buscar(&repo, 2)->codigo, "GLO2");
    checar_int("estatico: busca id=99 -> NULL", repo_estatico_buscar(&repo, 99) == NULL, 1);
    checar_int("estatico: remove id=2", repo_estatico_remover(&repo, 2), 1);
    checar_int("estatico: remover de novo falha", repo_estatico_remover(&repo, 2), 0);
    checar_int("estatico: quantidade 2 apos remocao", repo_estatico_quantidade(&repo), 2);
    checar_int("estatico: reaproveita posicao livre", repo_estatico_inserir(&repo, novo_voo(4, "X4", PRIO_REGULAR)), 1);
    checar_int("estatico: listar devolve 3", repo_estatico_listar(&repo, lista, CAPACIDADE_MAX), 3);
    checar_int("estatico: X4 ocupou a posicao do voo 2", lista[1].id, 4);
}

static void testar_dinamico(void)
{
    RepositorioDinamico repo;
    Voo lista[200];

    repo_dinamico_criar(&repo, 0);
    checar_int("dinamico: comeca vazio", repo_dinamico_quantidade(&repo), 0);
    for (int i = 1; i <= 150; i++)                 /* passa bem do limite estatico */
        repo_dinamico_inserir(&repo, novo_voo(i, "VOO", PRIO_REGULAR));
    checar_int("dinamico: aceita 150 voos (sem limite fixo)", repo_dinamico_quantidade(&repo), 150);
    checar_int("dinamico: nunca fica cheio", repo_dinamico_esta_cheio(&repo), 0);
    checar_int("dinamico: busca id=75", repo_dinamico_buscar(&repo, 75)->id, 75);
    checar_int("dinamico: busca id=999 -> NULL", repo_dinamico_buscar(&repo, 999) == NULL, 1);
    checar_int("dinamico: remove o primeiro no (id=150)", repo_dinamico_remover(&repo, 150), 1);
    checar_int("dinamico: remove um no do meio (id=75)", repo_dinamico_remover(&repo, 75), 1);
    checar_int("dinamico: remove o ultimo no (id=1)", repo_dinamico_remover(&repo, 1), 1);
    checar_int("dinamico: remover inexistente falha", repo_dinamico_remover(&repo, 75), 0);
    checar_int("dinamico: 147 apos 3 remocoes", repo_dinamico_quantidade(&repo), 147);
    checar_int("dinamico: listar devolve 147", repo_dinamico_listar(&repo, lista, 200), 147);
    checar_int("dinamico: insercao no inicio -> primeiro listado e o id 149", lista[0].id, 149);

    repo_dinamico_destruir(&repo);
    checar_int("dinamico: vazio apos destruir", repo_dinamico_quantidade(&repo), 0);
}

int main(void)
{
    testar_estatico();
    testar_dinamico();
    return teste_resumo();
}
