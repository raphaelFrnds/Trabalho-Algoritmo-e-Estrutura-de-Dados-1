# Relatório Técnico — Módulo 1: Modularização
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## 1. Funções implementadas (assinatura, parâmetros e retorno)

### `utils.h`
| Função | Parâmetros | Retorno | Descrição |
|---|---|---|---|
| `gerar_id_voo()` | nenhum | `int` | ID sequencial único, incrementado a cada chamada |
| `resetar_contador_id()` | nenhum | `void` | Volta o contador para 1 (usado nos testes) |
| `formatar_hora_chegada(destino, tam, momento)` | `char *`, `size_t`, `const struct tm *` (ou `NULL`) | `void` | Escreve a data/hora no formato `DD/MM/AAAA HH:MM:SS` |

### `validacao.h`
| Função | Parâmetros | Retorno | Descrição |
|---|---|---|---|
| `validar_prioridade(nome)` | `const char *` | `bool` | Verifica se o nome é um dos 5 níveis de prioridade |
| `validar_codigo(codigo)` | `const char *` | `bool` | Código não vazio e dentro do tamanho máximo |
| `prioridade_numerica(nome)` | `const char *` | `int` (1–5, ou -1) | Converte o nome no nível numérico |
| `nome_prioridade(nivel)` | `Prioridade` | `const char *` | Converte o nível no nome curto |

### `parametros_demo.h`
| Função | Parâmetros | Retorno | Descrição |
|---|---|---|---|
| `reclassificar_voo(voo, nova)` | `Voo *`, `Prioridade` | `void` | Altera o voo original (por referência) |
| `tentar_reclassificar_copia(voo, nova)` | `Voo`, `Prioridade` | `Prioridade` | Altera só a cópia local (por valor) |
| `trocar_valores(a, b)` | `int *`, `int *` | `void` | Swap que funciona (por referência) |
| `trocar_por_valor(a, b)` | `int`, `int` | `void` | Swap que não funciona (por valor) |

### `recursao_demo.h`
| Função | Parâmetros | Retorno | Descrição |
|---|---|---|---|
| `fatorial(n)` | `int` | `long long` (-1 se `n < 0`) | Fatorial recursivo |
| `fibonacci(n)` | `int` | `long long` (-1 se `n < 0`) | N-ésimo termo de Fibonacci, recursivo |
| `busca_binaria_recursiva(voos, id, inicio, fim)` | vetor de `Voo` ordenado por id, `int`, `int`, `int` | `int` (índice ou -1) | Busca recursiva O(log n) |

## 2. Passagem de parâmetros em C: por valor x por referência

Em C, **todo parâmetro é passado por valor**: a função sempre recebe uma
cópia. A "passagem por referência" é feita passando uma cópia do
**endereço** da variável (um ponteiro). Assim a função consegue alterar
a variável original através desse endereço.

- `tentar_reclassificar_copia(Voo voo, ...)` recebe uma cópia da struct
  inteira. Alterar `voo.prioridade` muda só a cópia. Depois da chamada,
  o voo original continua `regular`.
- `reclassificar_voo(Voo *voo, ...)` recebe o endereço. `voo->prioridade
  = nova` escreve na memória do voo original, então a alteração aparece
  fora da função.
- O mesmo vale para o swap: `trocar_por_valor(int a, int b)` troca só as
  cópias, e `trocar_valores(int *a, int *b)` troca os valores originais.

Isso foi comprovado em `test_modulo1.c` (seção `testar_parametros`).

**Custo:** passar uma struct por valor copia todos os seus bytes (o
`Voo` tem cerca de 40 bytes). Por ponteiro, copia-se só o endereço (8
bytes em 64 bits). Por isso, a partir do Módulo 2, as funções recebem
`Voo *` ou `const Voo *`. O `const` indica que a função só lê o voo.

## 3. Níveis de prioridade de pouso (base do Módulo 5)

Os 5 níveis usados na fila de prioridade, centralizados em
`TABELA_PRIORIDADES` (`validacao.c`). Eles são inspirados na
fraseologia real de aviação (MAYDAY / PAN-PAN / *minimum fuel*):

| Nome | Descrição | Prioridade | Espera máx. recomendada |
|---|---|---|---|
| `mayday` | Emergência declarada | 1 (mais urgente) | 0 min (imediato) |
| `panpan` | Urgência (pane técnica, passageiro doente) | 2 | 5 min |
| `combustivel` | Combustível mínimo | 3 | 10 min |
| `regular` | Voo comercial normal | 4 | 30 min |
| `treinamento` | Treinamento / translado | 5 (menos urgente) | 60 min |

Essa tabela é a base da fila de prioridade do Módulo 5. Cada voo entra
na fila do seu nível, e a coluna "espera máx." fica disponível para uma
futura política de reavaliação, que está fora do escopo obrigatório.

## 4. Observações sobre a linguagem (C)

- **Gerenciamento de memória manual:** no Módulo 2 a comparação entre
  alocação estática e dinâmica é feita de verdade, com vetor de tamanho
  fixo x `malloc`/`free`, e os vazamentos são verificados com `valgrind`.
- **Sem exceções:** entradas inválidas retornam códigos de erro, por
  exemplo `prioridade_numerica("xyz") == -1`.
- **Strings com tamanho fixo:** `codigo` e `hora_chegada` são vetores
  `char[]` dentro da struct. `voo_criar` usa `snprintf`, que nunca
  escreve além do tamanho do vetor.

## 5. Estrutura planejada para os próximos módulos

- **Módulo 2:** TAD `Repositório de Voos` (versões estática e dinâmica).
- **Módulo 3:** TAD Lista (voos na ordem de chegada ao espaço aéreo).
- **Módulo 4:** TAD Pilha (desfazer/refazer reclassificações de prioridade).
- **Módulo 5:** TAD Fila de Prioridade, o centro do sistema: sequência de pousos.
- **Módulo 6:** Integração completa — Sistema de Controle de Pousos.
