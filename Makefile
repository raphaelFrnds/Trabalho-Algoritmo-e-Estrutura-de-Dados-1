# Makefile da raiz: roda os testes de todos os modulos
MODULOS = $(wildcard modulo*/)

test:
	@for d in $(MODULOS); do echo "== $$d =="; $(MAKE) -s -C $$d test || exit 1; done

memcheck:
	@for d in $(MODULOS); do [ "$$d" = "modulo1_semanas1-3_modularizacao/" ] && continue; echo "== $$d =="; $(MAKE) -s -C $$d memcheck || exit 1; done

clean:
	@for d in $(MODULOS); do $(MAKE) -s -C $$d clean; done

.PHONY: test memcheck clean
