# Relatório Técnico Final — Sistema de Controle de Pousos
**Linguagem:** C (C11)

## 1. Visão geral do sistema

O sistema simula o trabalho da torre de controle na aproximação final:
voos chegam ao espaço aéreo, recebem um nível de prioridade (de
`mayday` a `treinamento`), pousam por ordem de urgência e ficam
registrados no histórico. Ele integra três estruturas estudadas na
disciplina, cumprindo o requisito do Módulo 6 de "combinar ao menos duas
estruturas estudadas".

## 2. Estruturas integradas e por quê

| Estrutura | Módulo de origem | Por que resolve o problema |
|---|---|---|
| Fila de prioridade (5 filas FIFO, uma por nível) | Módulo 5 | Modela exatamente a regra operacional: urgência decide quem pousa primeiro e, no mesmo nível, a ordem de chegada é respeitada |
| Lista encadeada simples | Módulo 3 | O número de pousos no dia é imprevisível. Inserir no início é O(1) e deixa o histórico do mais recente ao mais antigo, sem capacidade pré-alocada |
| Pilha (desfazer/refazer) | Módulo 4 | Corrigir uma prioridade registrada errada é "desfazer a última decisão", ou seja, LIFO |

## 3. Interface pública do `SistemaControle`

| Função | Parâmetros | Retorno | Complexidade |
|---|---|---|---|
| `sistema_cadastrar_voo` | código, prioridade (`const char *`) | `Voo *` ou `NULL` | O(1) |
| `sistema_autorizar_pouso` | — | `Voo *` ou `NULL` | O(k) na fila + O(n) para remover o registro de espera + O(1) no histórico |
| `sistema_reclassificar_voo` | id, nova prioridade | `bool` | O(n): busca o registro + reconstrói a fila |
| `sistema_desfazer_reclassificacao` | id | `bool` | O(n) |
| `sistema_refazer_reclassificacao` | id | `bool` | O(n) |
| `sistema_voos_aguardando` | — | `int` | O(k) |
| `sistema_voos_por_nivel` | vetor `int[5]` | `void` | O(k) |
| `sistema_proximo_pouso` | — | `Voo *` ou `NULL` | O(k) |
| `sistema_posicao_na_fila` | id | `int` (-1 se não estiver) | O(n) |
| `sistema_historico` | vetor de destino, máx. | `int` | O(n) |
| `sistema_buscar_no_historico` | id | `Voo *` ou `NULL` | O(n) |

k = 5 níveis, uma constante.

## 4. Por que reclassificar custa O(n)

A `FilaPrioridade` separa os voos em 5 filas FIFO. Quando um voo muda
de nível, ele precisa "trocar de fila". Só que uma fila encadeada FIFO
remove apenas do início, não do meio. A solução foi **reconstruir a
fila inteira** a partir da lista de registros de espera, que está em
ordem de chegada.

Percorrer os registros na ordem de chegada tem um efeito importante:
dentro de cada nível, a ordem continua sendo a de chegada. Se `AAA1`
(que chegou primeiro) é promovido para `combustivel`, ele fica **à
frente** dos voos `combustivel` que chegaram depois. O teste
`testar_fifo_apos_reclassificacao` confirma isso.

**Justificativa:** reclassificações são raras comparadas a registrar e
autorizar pousos. Pagar O(n) de vez em quando para manter as operações
frequentes simples é um bom trade-off, já que um aeroporto tem dezenas
de voos em espera, não milhões.

## 5. Gerenciamento de memória

| Momento | O que acontece |
|---|---|
| Cadastro | `malloc` do `Voo` e do `RegistroEspera`. Se um deles falhar, os dois são liberados (`free(NULL)` é seguro) e a função devolve `NULL` |
| Reconstrução da fila | Libera só os **nós** da fila antiga. Os voos continuam vivos |
| Pouso | O `RegistroEspera` é liberado e o `Voo` passa para o histórico, sem cópia |
| `sistema_destruir` | Libera os voos em espera + registros, depois os nós da fila, depois cada voo do histórico + nós da lista |

Verificação com `valgrind --leak-check=full`: **0 bytes perdidos, 0
erros**, tanto nos testes (`make memcheck`) quanto numa sessão completa
do menu (registrar, reclassificar, desfazer, pousar tudo, ver histórico
e sair).

## 6. Quando usar lista, pilha ou fila (síntese)

| Estrutura | Quando usar | Custo típico |
|---|---|---|
| Lista (vetor ou encadeada) | Dados que precisam ser percorridos, buscados ou listados, sem regra de prioridade | O(n) fora das extremidades |
| Pilha (LIFO) | A última ação precisa poder ser desfeita antes das outras | O(1) em tudo |
| Fila (FIFO) | A ordem de chegada precisa ser respeitada | O(1) com vetor circular ou encadeada |
| Fila de prioridade | Um critério de urgência passa na frente da ordem de chegada | O(1)/O(k) com poucos níveis discretos; O(log n) com heap |

## 7. Testes de integração

27 testes: fluxo completo (cadastro → pouso por prioridade → histórico
do mais recente ao mais antigo), cadastro inválido, pouso com fila
vazia, reclassificação mudando a ordem, desfazer restaurando a ordem
original, refazer, reclassificação de id inexistente ou com prioridade
inválida, voo que já pousou não pode mais ser reclassificado, FIFO
preservado após a reconstrução, busca no histórico e contagem por nível.
Ver `test_final.c`.

## 8. Limitações e trabalhos futuros

- Sem promoção automática por tempo de espera (discutido no relatório
  do Módulo 5).
- Sem persistência: os dados existem só durante a execução.
- Uma única pista. Com várias pistas, bastaria uma fila de prioridade
  por pista ou uma lista circular para o rodízio de pistas (Módulo 3).
- A reconstrução O(n) poderia virar O(log n) com um heap indexado, se o
  volume de reclassificações crescesse muito. Não foi necessário neste
  escopo.
