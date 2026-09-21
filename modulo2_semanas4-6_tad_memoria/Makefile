# Makefile - Modulo 2
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c repositorio_estatico.c repositorio_dinamico.c

all: test_modulo2

test_modulo2: test_modulo2.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test: test_modulo2
	./test_modulo2

# Confere se todo malloc teve o seu free (Linux/WSL)
memcheck: test_modulo2
	valgrind --leak-check=full --error-exitcode=1 ./test_modulo2

clean:
	rm -f test_modulo2 *.exe

.PHONY: all test memcheck clean
