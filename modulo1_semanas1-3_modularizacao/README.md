# Entregável 1 — Módulo 1 (Semanas 1–3): Modularização
**Projeto:** Sistema de Controle de Pousos (Torre de Controle)
**Linguagem:** C (C11)

## O que este entregável contém

Biblioteca de funções utilitárias modularizada, que serve de base para
o sistema de controle de pousos construído nos módulos seguintes.

| Arquivo | Semana | Conteúdo |
|---|---|---|
| `voo.h` / `voo.c` | — | Struct `Voo` e enum `Prioridade` (vira TAD no Módulo 2) |
| `utils.h` / `utils.c` | 1 | Geração de ID sequencial e formatação de data/hora |
| `validacao.h` / `validacao.c` | 1 | Validação do código do voo e da prioridade de pouso |
| `parametros_demo.h` / `.c` | 2 | Passagem por valor x por referência (ponteiros) |
| `recursao_demo.h` / `.c` | 3 | Fatorial, Fibonacci e busca binária recursiva |
| `main.c` | — | Junta tudo em um fluxo de demonstração |
| `test_modulo1.c` | — | Testes automatizados (31 casos) |
| `teste.h` | — | Mini-framework de testes (`checar_int`, `checar_str`) |
| `Makefile` | — | Compilação, execução e testes |

## Como executar

Requer apenas um compilador C (gcc ou clang), sem bibliotecas externas.

```bash
make run     # compila e roda a demonstração
make test    # compila e roda os testes
```

Sem `make` (ex.: Windows com MinGW):

```bash
gcc -std=c11 -Wall -Wextra -o main main.c voo.c utils.c validacao.c parametros_demo.c recursao_demo.c
gcc -std=c11 -Wall -Wextra -o test_modulo1 test_modulo1.c voo.c utils.c validacao.c parametros_demo.c recursao_demo.c
```

No **OnlineGDB**: selecione a linguagem C, envie todos os `.c` e `.h`
da pasta e deixe só **um** arquivo com `main` no projeto (`main.c` ou
`test_modulo1.c`).

## Decisões de projeto

- **Um `.h` e um `.c` por responsabilidade:** o `.h` é a interface
  pública (o que o resto do programa pode usar) e o `.c` é a
  implementação. Nada de um `main.c` com tudo dentro.
- **Prioridade como `enum`:** o valor numérico do enum já é a
  prioridade (1 = mais urgente). No Módulo 5, isso vira o índice direto
  do vetor de filas (`nivel - 1`), sem nenhuma conversão extra.
- **Sem exceções em C:** erros são sinalizados por valor de retorno
  (`-1`, `false`, `NULL`), e cada função documenta o seu no `.h`.
- **Contador de ID `static`:** a variável `proximo_id` só é visível
  dentro de `utils.c`, então nenhum outro arquivo consegue alterá-la
  por engano.
