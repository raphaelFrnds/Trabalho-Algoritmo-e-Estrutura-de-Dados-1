# Relatório Técnico — Módulo 4: Pilhas
**Projeto:** Sistema de Controle de Pousos | **Linguagem:** C (C11)

## 1. TAD Pilha — interface pública

`criar`, `empilhar(item)`, `desempilhar(&saida)`, `topo(&saida)`,
`esta_vazia`, `tamanho`, mais `esta_cheia` (vetor) e `destruir`
(encadeada). A interface é a mesma nas duas versões.

## 2. Complexidade das operações

| Operação | Pilha com Vetor | Pilha Encadeada |
|---|---|---|
| Empilhar (*push*) | O(1): escreve em `itens[topo++]` | O(1): novo nó vira o topo |
| Desempilhar (*pop*) | O(1): lê `itens[--topo]` | O(1): remove o nó do topo + `free` |
| Consultar topo | O(1) | O(1) |
| Verificar vazia | O(1) | O(1) |
| Destruir | não se aplica | O(n) |

Diferente da lista do Módulo 3, a pilha só mexe numa extremidade. Por
isso as duas versões são O(1) em tudo. A diferença real está na memória:

| | Vetor | Encadeada |
|---|---|---|
| Limite | Fixo (`PILHA_CAP` = 50) | Só a memória do computador |
| Custo por item | 4 bytes (um `enum`) | nó de 16 bytes + cabeçalho do `malloc` |
| Chamadas ao sistema | nenhuma | um `malloc`/`free` por operação |

Na aplicação da Semana 11, o histórico de cada voo é curto (poucas correções), então
o vetor é suficiente e mais simples. A encadeada é a opção quando não dá
para prever o tamanho.

## 3. Aplicação prática: desfazer/refazer de reclassificação

**Problema real:** em horário de pico, o controlador pode registrar a
prioridade errada de um voo. O sistema deixa reverter (e reaplicar) a
última decisão sem recadastrar o voo.

**Como funciona:**
- `gerenciador_reclassificar` empilha a prioridade **atual** em
  `desfazer` e só então aplica a nova, através do ponteiro `Voo *`.
- `gerenciador_desfazer` tira o topo de `desfazer`, aplica ao voo e
  empilha em `refazer` a prioridade que estava valendo.
- `gerenciador_refazer` faz o caminho inverso.
- Uma reclassificação nova **esvazia** `refazer`, porque uma ação nova
  invalida o "futuro" anterior.

Exemplo verificado em `test_modulo4.c`:

```
regular → combustivel → panpan      (2 reclassificações)
desfazer → combustivel
desfazer → regular
refazer  → combustivel
reclassificar para mayday           (refazer é descartado)
desfazer → combustivel
```

**Complexidade:** todas as operações são O(1).

## 4. Testes realizados

24 casos: pilha vazia (desempilhar/topo falham sem travar o programa),
ordem LIFO, limite da pilha com vetor, 200 itens na encadeada (sem
limite), liberação com `destruir`, e o fluxo completo de
reclassificar → desfazer → desfazer → refazer → nova reclassificação
(conferindo o descarte do refazer). `valgrind`: 0 vazamentos.
