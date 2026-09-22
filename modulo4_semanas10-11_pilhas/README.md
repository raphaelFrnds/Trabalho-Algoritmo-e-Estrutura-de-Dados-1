# Entregável 4 — Módulo 4 (Semanas 10–11): Pilhas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## Conteúdo

| Arquivo | Semana | Conteúdo |
|---|---|---|
| `pilha_array.h` / `.c` | 10 | TAD Pilha com vetor de tamanho fixo |
| `pilha_encadeada.h` / `.c` | 10 | TAD Pilha com nós encadeados (`malloc`/`free`) |
| `undo_redo_reclassificacao.h` / `.c` | 11 | Aplicação: desfazer/refazer reclassificações de prioridade |
| `test_modulo4.c` | — | Testes das pilhas e do desfazer/refazer (24 casos) |
| `relatorio.md` | — | Relatório técnico |

## Aplicação escolhida (Semana 11)

Das 3 opções do roteiro (expressões pós-fixadas, balanceamento de
parênteses ou undo/redo), foi escolhida **undo/redo**, porque se aplica
direto ao problema do projeto. Numa torre de controle movimentada, o
controlador pode reclassificar um voo por engano (marcar `panpan`
quando era `combustivel`, por exemplo) e precisa desfazer essa ação na
hora. Se desistir, também precisa conseguir refazer.

## Como executar

```bash
make test       # compila e roda os testes
make memcheck   # confere vazamentos com valgrind (Linux/WSL)
```

Sem `make`:

```bash
gcc -std=c11 -Wall -Wextra -o test_modulo4 test_modulo4.c voo.c utils.c validacao.c pilha_array.c pilha_encadeada.c undo_redo_reclassificacao.c
```

## Decisões de projeto

- As duas pilhas têm a mesma interface (`criar`, `empilhar`,
  `desempilhar`, `topo`, `esta_vazia`, `tamanho`), com prefixos
  `pilha_array_*` e `pilha_enc_*`.
- **Sem exceções:** `desempilhar` e `topo` devolvem `bool` e entregam o
  valor por um parâmetro de saída (`Prioridade *saida`). Assim, o erro
  "pilha vazia" nunca se confunde com um valor válido.
- O gerenciador guarda um **`Voo *`** e altera a prioridade do voo
  original através do ponteiro. É a passagem por referência do Módulo 1
  sendo usada na prática.
- Duas pilhas (`desfazer` e `refazer`): toda reclassificação nova limpa
  o `refazer`, como em qualquer editor de texto.
- O gerenciador usa a **pilha com vetor** (50 posições): não há `malloc`
  e cada voo tem um histórico curto. A encadeada fica como alternativa
  sem limite.
