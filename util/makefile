PROG=rmd160test
OBJS=ripemd160.o sha256.o
all: $(PROG) libutil.a
test: $(PROG) util.a; ./$<
util.a: $(OBJS); rm -fr $@ && ar r $@ $^$>
$(PROG): $(OBJS)
*.o: *.h
lib%.a: %.a; cp $< $@ && cp $@ ..
clean: ; rm -fr *.{o,a,so} $(PROG)
