# Makefile - Modulo 5
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c fila_array_circular.c fila_encadeada.c fila_prioridade.c

all: simulacao_pousos test_modulo5

simulacao_pousos: simulacao_pousos.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test_modulo5: test_modulo5.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

run: simulacao_pousos
	./simulacao_pousos

test: test_modulo5
	./test_modulo5

memcheck: test_modulo5 simulacao_pousos
	valgrind --leak-check=full --error-exitcode=1 ./test_modulo5
	valgrind --leak-check=full --error-exitcode=1 ./simulacao_pousos

clean:
	rm -f simulacao_pousos test_modulo5 *.exe

.PHONY: all run test memcheck clean
