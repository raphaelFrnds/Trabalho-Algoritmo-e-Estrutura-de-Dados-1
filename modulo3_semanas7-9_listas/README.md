# Entregável 3 — Módulo 3 (Semanas 7–9): Listas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## Conteúdo

| Arquivo | Semana | Conteúdo |
|---|---|---|
| `lista_array.h` / `.c` | 7 | Lista sequencial (vetor de tamanho fixo) |
| `lista_encadeada_simples.h` / `.c` | 8 | Lista encadeada simples (um ponteiro `proximo`) |
| `lista_dupla_circular.h` / `.c` | 9 | Lista duplamente encadeada, com modo circular opcional |
| `test_modulo3.c` | — | Testes das 4 variações (28 casos) |
| `relatorio.md` | — | Análise de complexidade comparada |

## Contexto de uso no sistema de pousos

Estas listas representam, por exemplo, os voos que entraram no espaço
aéreo do aeroporto em ordem de chegada (antes da priorização por
urgência, que só entra no Módulo 5) ou o histórico de pousos já
realizados no dia. A versão circular serve para rodízio, como a escala
de pistas ou de controladores de um turno.

## Como executar

```bash
make test       # compila e roda os testes
make memcheck   # confere vazamentos com valgrind (Linux/WSL)
```

Sem `make`:

```bash
gcc -std=c11 -Wall -Wextra -o test_modulo3 test_modulo3.c voo.c utils.c validacao.c lista_array.c lista_encadeada_simples.c lista_dupla_circular.c
```

## Decisões de projeto

- **Mesma interface nas 4 variações:** `criar`, `inserir`, `remover`,
  `buscar`, `percorrer`, `tamanho`, `esta_vazia` (e `destruir` nas
  encadeadas). Assim, a comparação pedida no roteiro fica só na
  estrutura interna.
- **As listas guardam `Voo *`, não cópias:** a lista organiza a ordem
  dos voos, mas não é dona deles. `destruir` libera só os nós, e quem
  criou o voo decide quando liberá-lo. É o mesmo modelo usado no
  Módulo 6.
- **`POSICAO_FIM` (-1)** indica "inserir no final", já que C não tem
  parâmetro opcional.
- **Dupla e circular na mesma struct** (campo `circular`), reaproveitando
  toda a navegação bidirecional. No modo circular, os percursos param
  depois de `tamanho` nós, senão nunca terminariam.
- Na dupla, `no_na_posicao` começa pela ponta mais próxima (início ou
  fim), o que corta pela metade o percurso no pior caso.
