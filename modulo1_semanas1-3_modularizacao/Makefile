# Makefile - Modulo 1
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

BASE = voo.c utils.c validacao.c parametros_demo.c recursao_demo.c

all: main test_modulo1

main: main.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

test_modulo1: test_modulo1.c $(BASE)
	$(CC) $(CFLAGS) -o $@ $^

run: main
	./main

test: test_modulo1
	./test_modulo1

clean:
	rm -f main test_modulo1 *.exe

.PHONY: all run test clean
