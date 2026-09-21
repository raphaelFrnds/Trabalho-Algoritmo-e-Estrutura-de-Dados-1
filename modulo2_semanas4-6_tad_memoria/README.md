# Entregável 2 — Módulo 2 (Semanas 4–6): TAD e Memória
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## Conteúdo

| Arquivo | Semana | Conteúdo |
|---|---|---|
| `tad_repositorio_voos_spec.md` | 4 | Especificação da interface do TAD (sem implementação) |
| `repositorio_estatico.h` / `.c` | 5 | Implementação com vetor de tamanho fixo (sem `malloc`) |
| `repositorio_dinamico.h` / `.c` | 6 | Implementação com nós alocados por `malloc`/`free` |
| `test_modulo2.c` | — | Mesmo roteiro de testes aplicado às duas versões (29 casos) |
| `relatorio.md` | — | Relatório técnico comparativo |

`voo`, `validacao`, `utils` e `teste.h` foram copiados do Módulo 1 sem
alterações.

## Como executar

```bash
make test       # compila e roda os testes
make memcheck   # roda os testes dentro do valgrind (Linux/WSL)
```

Sem `make`:

```bash
gcc -std=c11 -Wall -Wextra -o test_modulo2 test_modulo2.c voo.c utils.c validacao.c repositorio_estatico.c repositorio_dinamico.c
```

## Decisões de projeto

- **Estática de verdade:** `RepositorioEstatico` contém `Voo
  voos[CAPACIDADE_MAX]`, com tamanho fixo em tempo de compilação. O
  teste declara o repositório como variável local e nenhum `malloc` é
  feito.
- **Dinâmica de verdade:** cada inserção faz `malloc(sizeof(NoVoo))`,
  cada remoção faz `free`, e `repo_dinamico_destruir` libera todos os
  nós que sobraram. O `valgrind` confirma **0 bytes perdidos**.
- **Mesma interface, prefixos diferentes:** C não permite duas funções
  com o mesmo nome no mesmo programa, então as versões usam
  `repo_estatico_*` e `repo_dinamico_*`, com as mesmas operações e os
  mesmos contratos.
- Veja `relatorio.md` para a comparação completa.
