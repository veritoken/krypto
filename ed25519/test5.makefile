test5.js: test5.cpp
	docker run -v `pwd`:`pwd`: -w `pwd` em emcc --bind -o test5.js test5.cpp ed25519/*.c test2.c -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s LINKABLE=1 -s EXPORT_ALL=1
