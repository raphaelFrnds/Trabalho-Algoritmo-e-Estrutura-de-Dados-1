# Interface gráfica (GTK 3) — Sistema de Controle de Pousos

Versão com janela do mesmo sistema do Módulo 6. As estruturas de dados
são **exatamente as mesmas**: fila de prioridade, lista encadeada e
pilha. Só a interface mudou.

| Arquivo | Descrição |
|---|---|
| `interface_gtk.c` | Toda a janela: botões, tabelas e ações |
| `sistema_controle.h` / `.c` | O mesmo do Módulo 6, sem nenhuma alteração |
| demais `.c` / `.h` | Base dos Módulos 1, 3, 4 e 5, copiados sem alteração |

## Como rodar (WSL / Ubuntu / Linux)

```bash
sudo apt install libgtk-3-dev -y
make
./controle_pousos_gui
```

No Windows 11, a janela abre sozinha pelo WSLg. Se não abrir, rode
`echo $DISPLAY`: se vier vazio, seu Windows é 10 e precisa de um
servidor X (VcXsrv) ou de atualizar o WSL com `wsl --update`.

## O que a janela faz

- **Registrar voo:** digite o código, escolha a prioridade e clique em
  *Registrar voo*.
- **Fila de espera:** mostra os voos já na ordem real de pouso. A
  posição 0 é o próximo. Urgência vem primeiro e, dentro do mesmo
  nível, vale a ordem de chegada.
- **Autorizar próximo pouso:** tira o primeiro da fila e manda para o
  histórico.
- **Reclassificar / Desfazer / Refazer:** clique em um voo da fila,
  escolha a nova prioridade e use os botões. É o undo/redo do Módulo 4
  funcionando com dois cliques.
- **Histórico de pousos:** do mais recente para o mais antigo, igual à
  lista encadeada do Módulo 3.

## Observações para a apresentação

- O `interface_gtk.c` só chama funções do `sistema_controle.h`. Nenhuma
  estrutura de dados precisou mudar para a interface virar gráfica, o
  que é justamente o ponto da modularização do Módulo 1.
- A tabela da fila é redesenhada a cada ação, percorrendo os voos em
  espera nível por nível. Assim a tela mostra a mesma ordem que o
  `sistema_autorizar_pouso` vai seguir.
- O `valgrind` oficial do projeto continua sendo o do Módulo 6
  (`make memcheck` lá). O GTK mantém caches internos durante toda a
  execução, então ele sempre reporta blocos "still reachable", que não
  são vazamentos do nosso código.
