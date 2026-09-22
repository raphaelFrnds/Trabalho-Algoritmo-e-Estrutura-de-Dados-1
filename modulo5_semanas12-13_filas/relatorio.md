# Relatório Técnico — Módulo 5: Filas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## 1. TAD Fila — interface pública

`criar`, `enfileirar(voo)`, `desenfileirar(&saida)`, `frente()`,
`esta_vazia`, `tamanho`, `destruir`, mais `esta_cheia` na versão
circular e `posicao(id)` na encadeada (usada pela fila de prioridade).

## 2. Complexidade: vetor circular x lista encadeada

| Operação | Vetor Circular (capacidade fixa) | Lista Encadeada (sem limite) |
|---|---|---|
| Enfileirar | O(1): posição `(inicio + quantidade) % capacidade` | O(1): insere no fim, ponteiro `fim` mantido |
| Desenfileirar | O(1): `inicio = (inicio + 1) % capacidade` | O(1): remove do início + `free` |
| Consultar frente | O(1) | O(1) |
| Verificar cheia | O(1) | não se aplica (sem limite) |
| Memória | 1 `malloc` na criação | 1 `malloc` por voo enfileirado |

**Por que vetor *circular* e não um vetor comum?** Num vetor comum,
desenfileirar custaria O(n), porque seria preciso deslocar todos os
elementos uma posição para a frente. O vetor circular evita isso: os
índices "dão a volta" com o operador `%` e reaproveitam as posições
liberadas no começo. O teste `circular: o novo item deu a volta (indice
0)` confirma esse comportamento. O preço é ter uma capacidade fixa.

**Trade-off:** a circular é ideal quando existe um limite físico, como
o número de posições de espera (*holding*) que o espaço aéreo comporta.
A encadeada é melhor quando a demanda é imprevisível, que é o caso real
de um aeroporto com atrasos em cascata.

## 3. Fila de Prioridade de pouso

### Estrutura escolhida
`FilaPrioridade` é um vetor de 5 `FilaEncadeada`, uma por nível
(`niveis[0]` = `mayday` … `niveis[4]` = `treinamento`). Nenhuma
biblioteca pronta foi usada.

### Complexidade
| Operação | Complexidade | Justificativa |
|---|---|---|
| `fila_prio_enfileirar` | O(1) | Acesso direto a `niveis[prioridade - 1]` |
| `fila_prio_desenfileirar` | O(k), k = 5 | Percorre os níveis do mais urgente ao menos urgente até achar um não vazio |
| `fila_prio_proximo` | O(k) | Mesma lógica, sem remover |
| `fila_prio_tamanho` | O(k) | Soma as 5 quantidades (cada uma é O(1)) |
| `fila_prio_posicao_estimada` | O(n) | Soma os níveis mais urgentes + posição dentro do nível |

Como k = 5 é uma constante que não cresce com o número de voos,
`desenfileirar` na prática é **O(1)**.

**Por que não um heap binário?** Um heap dá O(log n) para inserir e
remover com prioridades arbitrárias. Aqui existem só 5 níveis
discretos, então uma fila por nível é:
- **mais simples** de implementar e de auditar (importante num sistema
  de segurança operacional);
- **mais rápida** (O(1) real, sem custo logarítmico);
- **justa por construção:** preserva a ordem de chegada dentro do mesmo
  nível. Um heap comum **não é estável**, ou seja, dois voos
  `regular` poderiam sair fora de ordem sem um campo extra de
  desempate.

## 4. Discussão sobre *starvation*

**Risco:** um voo `treinamento` ou `regular` poderia, em teoria, esperar
indefinidamente se voos mais urgentes chegassem sem parar.

**Decisão adotada (escopo enxuto):** a promoção automática por tempo de
espera (por exemplo, promover um `regular` para `combustivel` depois de
30 minutos em espera) **não foi implementada**. Ela fica registrada
como *trabalho futuro*, porque exigiria um relógio real ou simulado,
fora do escopo do roteiro. Isso também é coerente com a operação real:
quem decide mudar a prioridade é o piloto, que declara combustível
mínimo ou emergência quando a espera fica crítica, e não um algoritmo.
No sistema, isso aparece como a **reclassificação** do Módulo 6.

## 5. Testes e simulação

33 testes cobrindo: capacidade inválida, limite da fila circular,
reaproveitamento de posição (o índice dá a volta), FIFO na encadeada,
reutilização após `destruir` e, principalmente, a fila de prioridade:
`mayday` pousa antes de quem chegou primeiro, FIFO dentro do mesmo
nível, rejeição de nível inválido, contagem por nível e posição
estimada. A simulação (`simulacao_pousos.c`) com 8 voos teve a ordem de
pouso conferida manualmente (ver README). `valgrind`: 0 vazamentos nos
dois executáveis.
