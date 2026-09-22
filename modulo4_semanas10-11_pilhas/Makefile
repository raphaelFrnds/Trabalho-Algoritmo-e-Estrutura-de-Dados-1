# Makefile - Modulo 4
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c pilha_array.c pilha_encadeada.c undo_redo_reclassificacao.c

all: test_modulo4

test_modulo4: test_modulo4.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test: test_modulo4
	./test_modulo4

memcheck: test_modulo4
	valgrind --leak-check=full --error-exitcode=1 ./test_modulo4

clean:
	rm -f test_modulo4 *.exe

.PHONY: all test memcheck clean
