SUBDIRS=util ed25519 sign mine
#cworld: clean all; @echo ========================================
world: all; @echo ========================================
%:;	for f in $(SUBDIRS); do $(MAKE) -C $$f $@; done
clean:
	rm -fr lib*.a
	for f in $(SUBDIRS); do $(MAKE) -C $$f $@; done
