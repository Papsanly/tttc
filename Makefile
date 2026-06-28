.SILENT:

.PHONY: run test clean

run: build/main
	./build/main

test: build/test
	./build/test

build/main: $(wildcard src/*.c)
	mkdir -p build
	cc -g -o build/main src/main.c

build/test: $(wildcard src/*.c)
	mkdir -p build
	cc -g -DTEST -o build/test src/main.c

clean:
	rm -rf build
