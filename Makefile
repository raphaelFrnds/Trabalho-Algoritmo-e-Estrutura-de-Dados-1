# Makefile - Interface grafica (GTK 3)
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g $(shell pkg-config --cflags gtk+-3.0)
LIBS   = $(shell pkg-config --libs gtk+-3.0)

BASE = voo.c utils.c validacao.c pilha_array.c undo_redo_reclassificacao.c \
       fila_encadeada.c fila_prioridade.c lista_encadeada_simples.c sistema_controle.c

all: controle_pousos_gui

controle_pousos_gui: interface_gtk.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: controle_pousos_gui
	./controle_pousos_gui

clean:
	rm -f controle_pousos_gui *.exe

.PHONY: all run clean
