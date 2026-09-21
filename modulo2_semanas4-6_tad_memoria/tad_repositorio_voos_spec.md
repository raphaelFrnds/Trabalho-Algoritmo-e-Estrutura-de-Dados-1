# Especificação do TAD "Repositório de Voos" (Semana 4)

Como pede o roteiro, esta semana só **especifica** o TAD, sem
implementá-lo. A implementação vem nas Semanas 5 e 6.

## Interface pública (operações)

| Operação | Parâmetros | Retorno | Descrição |
|---|---|---|---|
| `criar(repo, capacidade)` | repositório, `int` | `bool` (sucesso) | Inicializa o repositório vazio |
| `inserir(repo, voo)` | repositório, `Voo` | `bool` (sucesso) | Insere um novo voo |
| `remover(repo, id)` | repositório, `int` | `bool` (sucesso) | Remove o voo pelo ID |
| `buscar(repo, id)` | repositório, `int` | `Voo *` ou `NULL` | Busca um voo pelo ID |
| `listar(repo, destino, max)` | repositório, vetor de `Voo`, `int` | `int` (quantidade copiada) | Copia todos os voos para `destino` |
| `esta_cheio(repo)` | repositório | `bool` | Indica se atingiu a capacidade (relevante na versão estática) |
| `quantidade(repo)` | repositório | `int` | Número de voos armazenados |
| `destruir(repo)` | repositório | `void` | Libera a memória (relevante na versão dinâmica) |

Em C não há sobrecarga de nomes, então as duas implementações usam
prefixos: `repo_estatico_*` e `repo_dinamico_*`. As operações e os
contratos são os mesmos nas duas.

## Encapsulamento

Quem usa o TAD só chama as funções acima e **nunca** acessa os campos
da struct diretamente. Por isso, o código cliente não precisa saber se
os voos estão num vetor de tamanho fixo ou numa lista de nós alocados
com `malloc`. Essa diferença interna é o que as Semanas 5 e 6 comparam:
duas implementações diferentes por trás da mesma interface.
