# Roteiro de Apresentação (10–15 min)
**Sistema de Controle de Pousos**

## Sugestão de tempo por seção

1. **Problema real (1–2 min):** a pista só recebe um pouso por vez, e
   ordem de chegada sozinha não serve quando um avião declara
   emergência ou está com pouco combustível.

2. **Estruturas usadas e por quê (3–4 min)**
   - Fila de prioridade (5 níveis) → decide quem pousa primeiro
   - Lista encadeada → histórico de pousos
   - Pilha (undo/redo) → corrigir uma prioridade registrada errada

3. **Demonstração ao vivo (5–6 min):** rodar `make run`:
   - Registrar 4–5 voos com prioridades variadas (deixar um `regular`
     chegar primeiro)
   - Mostrar a fila por nível (opção 3)
   - Reclassificar um `treinamento` para `mayday` ("o piloto declarou
     emergência") e mostrar que ele virou o próximo (opções 5 e 4)
   - Desfazer a reclassificação (opção 6)
   - Autorizar alguns pousos, mostrando que urgência vence ordem de
     chegada (opção 2)
   - Mostrar o histórico final (opção 8)
   - (Opcional) Rodar `make memcheck` e mostrar "no leaks are possible"

4. **Análise de complexidade (2 min):** tabela do relatório. Registrar
   é O(1), pousar é O(k) com k = 5, e reclassificar é O(n) de propósito.

5. **Limitações e trabalhos futuros (1 min):** sem promoção automática
   por tempo, sem persistência, uma pista só.

## Perguntas que a banca pode fazer (para se preparar)

- "Por que não usar um heap binário?"
  → Só existem 5 níveis discretos, então é O(1) real. Além disso, um
  heap comum não é estável: dois voos `regular` poderiam sair fora de
  ordem (relatório do Módulo 5, seção 3).
- "O que acontece se dois voos tiverem a mesma prioridade?"
  → FIFO dentro do nível, demonstrado na simulação do Módulo 5.
- "E se um voo `treinamento` ficar esperando para sempre?"
  → Discussão de *starvation* no relatório do Módulo 5, seção 4. Na
  operação real, o piloto declara combustível mínimo e o voo é
  reclassificado.
- "Quem libera a memória dos voos?"
  → O `SistemaControle`, em `sistema_destruir`. As estruturas só
  guardam ponteiros (relatório final, seção 5), e o `valgrind` confirma
  0 vazamentos.
- "Por que reclassificar reconstrói a fila inteira?"
  → Uma fila FIFO não remove do meio. É O(n), mas raro (relatório
  final, seção 4).
