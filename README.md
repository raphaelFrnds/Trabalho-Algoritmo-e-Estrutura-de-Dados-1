# Sistema de Controle de Pousos (Torre de Controle)

**Disciplina:** Algoritmos e Estrutura de Dados I (Engenharia de Software) | **Linguagem:** C (C11), sem bibliotecas externas

Uma pista só recebe um pouso por vez, e em horário de pico vários
aviões ficam esperando no ar. A ordem de chegada sozinha não resolve,
porque um avião que declara emergência ou está com pouco combustível
precisa pousar antes, mesmo que tenha chegado depois. Este projeto
simula a **sequência de pousos** decidida pela torre de controle, com 5
níveis de prioridade inspirados na fraseologia real de aviação
(MAYDAY, PAN-PAN, *minimum fuel*).

Ele é o projeto integrador da disciplina: cada módulo do repositório
corresponde a um entregável do semestre, e o projeto final (Módulo 6)
junta as estruturas estudadas em um sistema único e funcional.

## Níveis de prioridade

| Nome | Descrição | Prioridade |
|---|---|---|
| `mayday` | Emergência declarada | 1 (mais urgente) |
| `panpan` | Urgência (pane técnica, passageiro doente) | 2 |
| `combustivel` | Combustível mínimo | 3 |
| `regular` | Voo comercial normal | 4 |
| `treinamento` | Treinamento / translado | 5 (menos urgente) |

## Estrutura do repositório

```
controle-pousos/
├── modulo1_semanas1-3_modularizacao/     # Entregável 1 — funções, parâmetros (valor x ponteiro), recursão
├── modulo2_semanas4-6_tad_memoria/       # Entregável 2 — TAD com alocação estática x dinâmica (malloc/free)
├── modulo3_semanas7-9_listas/            # Entregável 3 — lista em vetor, encadeada simples, dupla e circular
├── modulo4_semanas10-11_pilhas/          # Entregável 4 — pilha + desfazer/refazer de reclassificação
├── modulo5_semanas12-13_filas/           # Entregável 5 — fila circular/encadeada + fila de prioridade
└── modulo6_semanas14-16_projeto_final/   # Projeto final — integra as estruturas acima
```

Cada pasta é **autocontida**: tem seus próprios `.h`/`.c` comentados,
`Makefile`, `README.md`, testes e um `relatorio.md` técnico, seguindo as
normas de entrega da disciplina. Por isso alguns arquivos-base (`voo`,
`validacao`, `utils`, `teste.h`) aparecem copiados em mais de uma
pasta. Isso é proposital: cada entrega pode ser compilada e corrigida
sozinha, na data do cronograma.

## Como rodar

Só é preciso um compilador C (gcc ou clang). No Windows, use MinGW ou
WSL, ou o [OnlineGDB](https://www.onlinegdb.com) (envie os `.c`/`.h`
da pasta, deixando apenas um arquivo com `main`).

```bash
# Testes de um módulo específico (exemplo: Módulo 5)
cd modulo5_semanas12-13_filas
make test

# Sistema final, interativo, no terminal
cd modulo6_semanas14-16_projeto_final
make run

# Da raiz: testes de TODOS os módulos
make test

# Da raiz: todos os testes dentro do valgrind (Linux/WSL)
make memcheck
```

Cada `README.md` de módulo também traz o comando `gcc` completo, para
quem não tem `make`.

**Resultado atual: 172 testes passando em 6 módulos, 0 falhas.**
Compila sem avisos com `-std=c11 -Wall -Wextra -pedantic -Werror` (gcc
e clang), sem vazamentos no `valgrind` e sem erros com
`-fsanitize=address,undefined`.

## Resumo por módulo

| Módulo | Semanas | Estrutura central | Testes |
|---|---|---|---|
| 1 — Modularização | 1–3 | Funções, parâmetros por valor x ponteiro, recursão | 31 |
| 2 — TAD e Memória | 4–6 | TAD estático (vetor fixo) x dinâmico (`malloc`/`free`) | 29 |
| 3 — Listas | 7–9 | Vetor, encadeada simples, dupla e circular | 28 |
| 4 — Pilhas | 10–11 | Pilha + desfazer/refazer de reclassificação | 24 |
| 5 — Filas | 12–13 | Fila circular/encadeada + **fila de prioridade** | 33 |
| 6 — Projeto Final | 14–16 | Integração: fila de prioridade + lista + pilha | 27 |

## Decisões de projeto

- **A fila de prioridade (Módulo 5, centro do Módulo 6) é um vetor de 5
  `FilaEncadeada`**, uma por nível. O `enum Prioridade` vai de 1 a 5,
  então o voo entra direto em `niveis[prioridade - 1]`, em O(1), e sai
  em O(k) com k = 5. Isso respeita FIFO dentro do mesmo nível, o que um
  heap comum não garante. Justificativa completa em
  `modulo5.../relatorio.md`.
- **O histórico de pousos (Módulos 3/6) é uma lista encadeada simples**,
  com inserção no início em O(1). O volume de pousos do dia é
  imprevisível, então não vale pré-alocar capacidade.
- **Quem é dono da memória fica claro:** no sistema final, o
  `SistemaControle` aloca cada `Voo` e o libera em `sistema_destruir`.
  Fila, lista e pilha só guardam ponteiros, então um voo passa da fila
  para o histórico sem cópia.
- **Erros sem exceções:** as funções retornam `bool`, `NULL` ou `-1`, e
  valores de saída vêm por ponteiro (`bool desempilhar(Pilha *p,
  Prioridade *saida)`).
- **Reclassificar custa O(n)** (reconstrói a fila a partir dos voos em
  espera, na ordem de chegada). É uma troca consciente: reclassificar é
  raro comparado a registrar e autorizar pousos.

## Limitações conhecidas

- Sem promoção automática de prioridade por tempo de espera (registrado
  como trabalho futuro nos relatórios dos Módulos 5 e 6).
- Sem persistência em disco: os dados existem só durante a execução.
- Uma pista só.

## Licença

Distribuído sob a licença MIT — veja [`LICENSE`](LICENSE).

---

Projeto desenvolvido para a disciplina de Algoritmos e Estrutura de
Dados I, com o roteiro de trabalho implementado em C.
