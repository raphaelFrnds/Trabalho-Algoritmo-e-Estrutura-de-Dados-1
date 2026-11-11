# Interface web (WebAssembly)

Terceira interface do Sistema de Controle de Pousos, feita para rodar no
navegador e ser publicada pelo **GitHub Pages**. Assim como o menu de
terminal (Módulo 6) e a janela GTK (`interface_grafica_gtk/`), ela é só uma
casca: a fila de prioridade, a lista encadeada e a pilha são **o mesmo código
C do Módulo 6**, compilado para WebAssembly com o Emscripten.

```
docs/index.html + estilo.css + app.js     página (só desenha a tela)
            │  chama
            ▼
interface_web/ponte_web.c                 ponte C <-> JavaScript
            │  chama
            ▼
modulo6_.../sistema_controle.h            núcleo: fila · lista · pilha
```

## Arquivos

| Arquivo | O que é |
|---|---|
| `interface_web/ponte_web.c` | Funções `web_*` chamadas pelo JavaScript. Devolve o estado do sistema como JSON. |
| `interface_web/Makefile` | Compila o núcleo do Módulo 6 + a ponte para `docs/nucleo.js`. |
| `docs/index.html`, `estilo.css`, `app.js` | A página. Não contém lógica de fila, lista ou pilha. |
| `docs/nucleo.js` | **Gerado** pelo `make` (o WebAssembly vai embutido). Não editar à mão. |

## Como usar

Abrir o site publicado, ou abrir `docs/index.html` direto no navegador
(funciona sem servidor e sem instalar nada).

## Como recompilar

Só é preciso se algum arquivo `.c` mudar.

```bash
sudo apt install emscripten   # uma vez
cd interface_web
make                          # gera ../docs/nucleo.js
make servir                   # opcional: http://localhost:8000
```

## Como publicar no GitHub Pages

No repositório: **Settings → Pages → Build and deployment → Deploy from a
branch**, escolher a branch `main` e a pasta `/docs`, e salvar. Em um ou dois
minutos o site fica disponível no endereço mostrado nessa mesma tela.
