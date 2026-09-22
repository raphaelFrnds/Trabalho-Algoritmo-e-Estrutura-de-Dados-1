# Makefile - Modulo 6 (Projeto Final)
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c pilha_array.c undo_redo_reclassificacao.c \
       fila_encadeada.c fila_prioridade.c lista_encadeada_simples.c sistema_controle.c

all: controle_pousos test_final

controle_pousos: interface_cli.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test_final: test_final.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

run: controle_pousos
	./controle_pousos

test: test_final
	./test_final

memcheck: test_final
	valgrind --leak-check=full --error-exitcode=1 ./test_final

clean:
	rm -f controle_pousos test_final *.exe

.PHONY: all run test memcheck clean
