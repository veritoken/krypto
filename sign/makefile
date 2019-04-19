# -*- Makefile -*-
PROG=sign
INCS=-I../util -I../ed25519
LIBS=-lutil -led25519
CFLAGS+=$(INCS)
LDFLAGS=-L.. $(LIBS) $(INCS)
all: clean $(PROG) testing
testing: $(PROG)
	@./sign                                                     || echo ==PASS1==
	@./sign -u --seed sdffdg                                    || echo ==PASS2==
	@cat Makefile | ./sign -u --seed makefile -v                || echo ==PASS3==
	@cat Makefile | ./sign -u --seed makefile -s >Makefile.sig  && echo ==PASS4==
	@cat Makefile.sig Makefile | ./sign -u --seed makefile -v   && echo ==PASS5==
	@cat Makefile | ./sign -u --seed makefile -S >Makefile.psig && echo ==PASS6==
	@cat Makefile.psig Makefile | ./sign -u -V --seed Makefile  || echo ==PASS7==
	@cat Makefile.psig Makefile | ./sign -u -V
lib%.a: %.a; cp $< $@ && cp $@ ..
#include makefile.inc
clean: ; rm -fr $(PROG) *sig

signage.js: signage.cpp
	docker run -v `pwd`:`pwd` -w `pwd` em emcc --bind -o $@ $< ../ed25519/ed25519/*.c ../ed25519/test2.c -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s LINKABLE=1 -s EXPORT_ALL=1
