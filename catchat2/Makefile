#DOCKER=docker run -v `pwd`/..:`pwd`/..: -w `pwd` em
DOCKER=/usr/local/bin/docker run -v `pwd`/..:`pwd`/..: -w `pwd` em
OPTS=-s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s LINKABLE=1 -s EXPORT_ALL=1
as: all serve
all: static static/test1.js static/test2.js static/test1.html static/test2.html
static: ; mkdir static
static/%.js: src/%.cpp
	$(DOCKER) emcc --bind -o $@ $< -I../ed25519 ../ed25519/ed25519/*.c $(OPTS)
static/%.html: html/%.html
	cp $< $@
clean:
	find . -name '*~' -o -name '*.pyc' | xargs rm
	rm -fr *.wasm *.js static
	tree .
serve: ; python -um ws
