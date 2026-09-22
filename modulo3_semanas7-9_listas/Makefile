# Makefile - Modulo 3
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c lista_array.c lista_encadeada_simples.c lista_dupla_circular.c

all: test_modulo3

test_modulo3: test_modulo3.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test: test_modulo3
	./test_modulo3

memcheck: test_modulo3
	valgrind --leak-check=full --error-exitcode=1 ./test_modulo3

clean:
	rm -f test_modulo3 *.exe

.PHONY: all test memcheck clean
