quick.js:
	docker run -v `pwd`:`pwd`: -w `pwd` em emcc --bind -o quick.js quick.cpp ed25519/*.c test2.c
