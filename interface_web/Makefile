# Makefile - Interface web (WebAssembly)
#
# Compila o MESMO nucleo em C do Modulo 6 para WebAssembly, usando o
# Emscripten (emcc). O resultado (nucleo.js) vai para a pasta docs/,
# que e o site publicado pelo GitHub Pages.
#
#   sudo apt install emscripten    (uma vez)
#   make                           (gera ../docs/nucleo.js)
#   make servir                    (abre o site em http://localhost:8000)

EMCC   = emcc
NUCLEO = ../modulo6_semanas14-16_projeto_final
SITE   = ../docs

BASE = $(NUCLEO)/voo.c $(NUCLEO)/utils.c $(NUCLEO)/validacao.c \
       $(NUCLEO)/pilha_array.c $(NUCLEO)/undo_redo_reclassificacao.c \
       $(NUCLEO)/fila_encadeada.c $(NUCLEO)/fila_prioridade.c \
       $(NUCLEO)/lista_encadeada_simples.c $(NUCLEO)/sistema_controle.c

# SINGLE_FILE embute o .wasm dentro do .js: o site funciona ate abrindo
# o index.html direto do disco, sem servidor.
EMFLAGS = -std=c11 -Wall -Wextra -O2 -I$(NUCLEO) \
          -s MODULARIZE=1 -s EXPORT_NAME=criarNucleo -s SINGLE_FILE=1 \
          -s ENVIRONMENT=web,node \
          -s EXPORTED_RUNTIME_METHODS=ccall,cwrap,UTF8ToString

all: $(SITE)/nucleo.js

$(SITE)/nucleo.js: ponte_web.c $(BASE)
	$(EMCC) $(EMFLAGS) -o $@ $^

servir: all
	cd $(SITE) && python3 -m http.server 8000

clean:
	rm -f $(SITE)/nucleo.js

.PHONY: all servir clean
