# Relatório Técnico — Módulo 2: TAD e Memória
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## 1. TAD implementado: Repositório de Voos

Interface pública (a mesma nas duas versões; ver
`tad_repositorio_voos_spec.md`): `criar`, `inserir`, `remover`,
`buscar`, `listar`, `esta_cheio`, `quantidade` (e `destruir` na
dinâmica).

O cliente (por exemplo, o sistema final do Módulo 6) usa apenas essas
funções. Por isso, ele não depende de como os voos estão guardados.

## 2. Complexidade das operações

| Operação | Versão Estática (vetor fixo) | Versão Dinâmica (nós com `malloc`) |
|---|---|---|
| Inserir | O(n): procura a primeira posição livre | O(1): insere sempre no início |
| Remover por id | O(n): busca linear | O(n): busca linear + `free` do nó |
| Buscar por id | O(n): busca linear | O(n): busca linear |
| Listar todos | O(capacidade) | O(n) |
| Verificar se está cheio | O(1) | O(1) (sempre `false`) |
| Destruir | não se aplica | O(n): um `free` por nó |

## 3. Comparação estática x dinâmica

| Critério | Estática | Dinâmica |
|---|---|---|
| Onde fica a memória | Dentro da struct (pilha de execução ou global) | No *heap*, um bloco por voo |
| Capacidade máxima | Fixa: `CAPACIDADE_MAX` (100) em tempo de compilação | Só limitada pela memória do computador |
| Desperdício | Reserva 100 × `sizeof(Voo)` mesmo com 1 voo | Aloca só o necessário |
| Custo extra por elemento | 1 `bool` de controle (`ocupado`) | 1 ponteiro `proximo` (8 bytes) + cabeçalho do `malloc` |
| Risco de erro | *Overflow* se não checar o limite | *Memory leak* se esquecer o `free`; *dangling pointer* se usar o nó depois do `free` |
| Previsibilidade | Alta: memória nunca cresce | Cresce e diminui conforme a demanda |

**Conclusão:** o número de aeronaves na área de controle varia muito ao
longo do dia (picos de manhã e à noite, com atrasos em cascata). Por
isso, a versão **dinâmica** é a mais adequada para o sistema: ela não
rejeita um voo por falta de espaço e não reserva memória à toa. A versão
estática faz sentido quando existe um limite físico conhecido, como o
número fixo de posições de estacionamento no pátio do aeroporto.

## 4. Gerenciamento de memória em C

- **`malloc` pode falhar:** `repo_dinamico_inserir` verifica se o
  retorno é `NULL` e, nesse caso, devolve `false` sem corromper a lista.
- **Ordem ao liberar:** em `repo_dinamico_destruir`, o ponteiro
  `proximo` é salvo **antes** do `free(atual)`. Ler `atual->proximo`
  depois do `free` seria acesso a memória já liberada (comportamento
  indefinido).
- **Verificação:** os testes foram executados com `valgrind
  --leak-check=full`, e o resultado foi *"All heap blocks were freed — no
  leaks are possible"*, com 0 erros.
- **Ponteiro devolvido por `buscar`:** aponta para dentro do repositório.
  Ele deixa de ser válido depois de remover aquele voo. Na versão
  dinâmica, o nó foi liberado; na estática, a posição pode ser reutilizada.

## 5. Testes realizados

29 casos: capacidade inválida rejeitada, inserção até o limite, rejeição
quando cheio, reaproveitamento de posição livre (estática), inserção de
150 voos sem limite (dinâmica), busca existente/inexistente, remoção do
primeiro, do meio e do último nó, e contagem após destruir. Ver
`test_modulo2.c`.
