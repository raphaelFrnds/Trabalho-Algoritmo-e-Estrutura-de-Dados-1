# Entregável 5 — Módulo 5 (Semanas 12–13): Filas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

Este é o módulo mais importante do projeto: a **fila de prioridade** é
o componente central de todo o sistema de controle de pousos.

## Conteúdo

| Arquivo | Semana | Conteúdo |
|---|---|---|
| `fila_array_circular.h` / `.c` | 12 | Fila FIFO com vetor circular, capacidade fixa |
| `fila_encadeada.h` / `.c` | 12 | Fila FIFO com nós encadeados, sem limite |
| `fila_prioridade.h` / `.c` | 13 | **Fila de prioridade de pouso (5 níveis)**, feita com 5 `FilaEncadeada` |
| `simulacao_pousos.c` | 13 | Simulação de um horário de pico com 8 voos |
| `test_modulo5.c` | — | Testes das 3 filas (33 casos) |
| `relatorio.md` | — | Relatório técnico |

## Como executar

```bash
make run        # roda a simulação
make test       # roda os testes
make memcheck   # testes + simulação dentro do valgrind (Linux/WSL)
```

Sem `make`:

```bash
gcc -std=c11 -Wall -Wextra -o simulacao_pousos simulacao_pousos.c voo.c utils.c validacao.c fila_array_circular.c fila_encadeada.c fila_prioridade.c
gcc -std=c11 -Wall -Wextra -o test_modulo5 test_modulo5.c voo.c utils.c validacao.c fila_array_circular.c fila_encadeada.c fila_prioridade.c
```

## Como a fila de prioridade funciona

Em vez de um heap binário, foi usada uma abordagem mais simples e fácil
de conferir: **um vetor com 5 `FilaEncadeada`** (a própria fila FIFO
desta semana), uma por nível de prioridade. Como o `enum Prioridade` vai
de 1 a 5, o voo entra direto em `niveis[prioridade - 1]`, sem busca e
sem conversão.

Isso garante duas propriedades ao mesmo tempo:

1. Entre níveis diferentes, o mais urgente pousa primeiro, mesmo que
   tenha chegado depois. Na simulação, o `mayday` chegou em 6º e pousou
   em 1º.
2. Dentro do mesmo nível, vale a ordem de chegada (FIFO). Dois voos
   `combustivel` pousam na ordem em que chegaram.

Resultado da simulação (`make run`):

```
 1. GLO1555  - mayday       (chegou em 6o)
 2. TAM3301  - panpan       (chegou em 4o)
 3. AZU4888  - panpan       (chegou em 8o)
 4. GLO1180  - combustivel  (chegou em 2o)
 5. AZU4700  - combustivel  (chegou em 5o)
 6. AZU4521  - regular      (chegou em 1o)
 7. TAM3090  - regular      (chegou em 7o)
 8. PTB2210  - treinamento  (chegou em 3o)
```

Veja `relatorio.md` para a análise de complexidade e a discussão sobre
*starvation*.
