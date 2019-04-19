# -*- Makefile -*-
PROG=mine
CFLAGS+=-I../util -I../ed25519
LDFLAGS=../util/util.a ../ed25519/ed25519.a
all: clean test
test: $(PROG)
	@echo %%%%%%%%%%%%%%%%%%%
	@./$(PROG) -b 15 -d 8 <makefile 1>1 2>2
	/bin/cat -vet 1 ; echo
	/bin/cat -vet 2
	@echo %%%%%%%%%%%%%%%%%%%
	@./$(PROG) -b 15 -d 16 <makefile 1>1 2>2
	/bin/cat -vet 1 ; echo
	/bin/cat -vet 2
	@echo %%%%%%%%%%%%%%%%%%%
clean:;	@rm -fr $(PROG) ?
