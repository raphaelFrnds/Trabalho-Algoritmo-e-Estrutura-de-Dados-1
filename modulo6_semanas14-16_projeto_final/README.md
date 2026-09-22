# Projeto Final — Módulo 6 (Semanas 14–16): Sistema de Controle de Pousos
**Linguagem:** C (C11), sem bibliotecas externas

## Sobre o projeto

Sistema da torre de controle que decide a **sequência de pousos** de um
aeroporto. Voos em emergência pousam antes dos outros, e voos com a
mesma prioridade pousam na ordem de chegada. O sistema integra três
estruturas de dados estudadas na disciplina:

| Estrutura | Papel no sistema |
|---|---|
| **Fila de prioridade** (Módulo 5) | Organiza os voos aguardando pouso por nível de urgência (obrigatório) |
| **Lista encadeada** (Módulo 3) | Histórico dos pousos realizados no dia (obrigatório) |
| **Pilha** (Módulo 4, via undo/redo) | Corrige uma prioridade registrada errada antes do pouso (extra) |

## Arquivos

| Arquivo | Descrição |
|---|---|
| `sistema_controle.h` / `.c` | `SistemaControle`: integra as 3 estruturas e cuida da memória dos voos |
| `interface_cli.c` | Menu interativo no terminal |
| `test_final.c` | 27 testes de integração |
| `relatorio.md` | Relatório técnico final |
| `apresentacao_roteiro.md` | Roteiro para a apresentação (10–15 min) |
| `voo`, `validacao`, `utils`, `teste.h` | Base (Módulo 1) |
| `pilha_array`, `undo_redo_reclassificacao` | Base (Módulo 4) |
| `fila_encadeada`, `fila_prioridade` | Base (Módulo 5) |
| `lista_encadeada_simples` | Base (Módulo 3) |

## Como executar

```bash
make run        # compila e abre o menu interativo
make test       # roda os testes de integração
make memcheck   # testes dentro do valgrind (Linux/WSL)
```

Sem `make` (Windows/MinGW):

```bash
gcc -std=c11 -Wall -Wextra -o controle_pousos interface_cli.c voo.c utils.c validacao.c pilha_array.c undo_redo_reclassificacao.c fila_encadeada.c fila_prioridade.c lista_encadeada_simples.c sistema_controle.c
```

No **OnlineGDB**: envie todos os `.c` e `.h` da pasta, **exceto**
`test_final.c`, porque só pode haver um `main` no projeto.

## Fluxo principal

1. **Registro:** o voo entra no espaço aéreo, recebe uma prioridade
   (`mayday` a `treinamento`) e entra na fila de prioridade.
2. **Pouso:** a torre autoriza sempre o próximo voo mais urgente,
   respeitando FIFO dentro do mesmo nível. O voo sai da fila e vai para
   o início do histórico (lista encadeada).
3. **Correção (extra):** se a prioridade mudar (o piloto declarou
   emergência, por exemplo) ou tiver sido registrada errada, o voo pode
   ser reclassificado enquanto ainda está em espera, com desfazer e
   refazer.

## Decisões de projeto

- **Quem é dono da memória:** o sistema faz `malloc` de cada `Voo` no
  cadastro e o `free` em `sistema_destruir`. A fila, a lista e o
  gerenciador só guardam ponteiros. Por isso, um voo pode passar da fila
  para o histórico sem ser copiado. O `valgrind` confirma **0
  vazamentos**, tanto nos testes quanto numa sessão completa do menu.
- **Registro de espera:** o Python teria um dicionário `id → voo`. C
  não tem dicionário nativo, então o sistema mantém uma lista encadeada
  auxiliar (`RegistroEspera`) na ordem de chegada, com o voo e o seu
  histórico de undo/redo. A busca por id é O(n), o que é aceitável para
  dezenas de voos em espera.
- **Reclassificar reconstrói a fila** (O(n)) a partir dos registros,
  na ordem de chegada. Assim, a regra FIFO dentro do nível continua
  valendo (há um teste para isso). Ver `relatorio.md`.
- **Promoção automática por tempo de espera** não foi implementada (ver
  a discussão de *starvation* no relatório do Módulo 5).
