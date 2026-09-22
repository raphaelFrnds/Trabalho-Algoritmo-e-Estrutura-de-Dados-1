# Relatório Técnico — Módulo 3: Listas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## 1. TAD Lista — interface pública

`criar`, `inserir(voo, posicao)`, `remover(posicao, &removido)`, `obter(posicao)` (simples),
`buscar(id)`, `percorrer(destino, max)`, `tamanho`, `esta_vazia` e
`destruir` (nas encadeadas). A dupla tem também `percorrer_reverso`.
Cada variação usa um prefixo: `lista_array_*`, `lista_simples_*` e
`lista_dupla_*`.

## 2. Complexidade das operações

| Operação | Vetor (`ListaArray`) | Encadeada Simples | Dupla (não circular) |
|---|---|---|---|
| Inserir no início | O(n): desloca todos para a direita | O(1) | O(1) |
| Inserir no final | O(1) | O(1): ponteiro `fim` mantido | O(1): ponteiro `fim` mantido |
| Inserir no meio (posição k) | O(n): desloca os elementos após k | O(n): anda até k | O(n): anda até k pela ponta mais próxima |
| Remover do início | O(n): desloca os restantes | O(1) | O(1) |
| Remover do final | O(1) | O(n): precisa achar o penúltimo | O(1): usa `fim->anterior` |
| Buscar por id | O(n) | O(n) | O(n) |
| Acesso por índice | O(1) | O(n) | O(n) |
| Percorrer ao contrário | O(n) (índices decrescentes) | não suportado (só há `proximo`) | O(n): via `anterior` |

## 3. Memória: vetor x encadeada

| | Vetor | Encadeada simples | Dupla |
|---|---|---|---|
| Por elemento | 1 ponteiro (8 bytes) | ponteiro do voo + `proximo` (16 bytes) + cabeçalho do `malloc` | ponteiro do voo + `proximo` + `anterior` (24 bytes) + cabeçalho |
| Capacidade | Fixa (`LISTA_CAP` = 100) | Ilimitada | Ilimitada |
| Localidade de cache | Ótima: elementos contíguos | Ruim: nós espalhados no *heap* | Ruim |

## 4. Quando usar cada uma

- **Vetor:** acesso por índice frequente e inserções/remoções
  concentradas no final. Também é a mais rápida de percorrer, porque os
  dados ficam juntos na memória.
- **Encadeada simples:** muitas inserções/remoções no início e tamanho
  imprevisível, como o **histórico de pousos** do Módulo 6, em que o
  mais recente entra no início em O(1).
- **Duplamente encadeada:** quando é preciso navegar nos dois sentidos,
  por exemplo para revisar os pousos do mais recente para o mais antigo
  e vice-versa. Custa um ponteiro a mais por nó.
- **Circular:** rodízio, como a escala de pistas (27L → 27R → 09L →
  volta para 27L) ou o revezamento de controladores no turno.

## 5. Cuidados específicos de C

- Todos os nós são liberados em `destruir`. A dupla circular percorre
  exatamente `tamanho` nós, porque um `while (no != NULL)` nunca
  terminaria numa lista circular.
- `valgrind --leak-check=full`: **0 vazamentos, 0 erros**.

## 6. Testes realizados

28 casos: lista vazia, inserção no início/meio/fim, posição inválida,
busca existente/inexistente, remoção do início/meio/fim, atualização do
ponteiro `fim` depois de remover o último, percurso nos dois sentidos
(dupla) e verificação dos ponteiros no modo circular (dar a volta
completa, religar depois de remover o início). Ver `test_modulo3.c`.
