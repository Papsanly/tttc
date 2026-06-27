.SILENT:

CC := cc
CFLAGS := -std=c2x -g -Wall -Wextra -Wconversion -Wpedantic
DEPS := $(wildcard src/*.c)

.PHONY: run test clean

run: build/main
	./build/main

test: build/test
	./build/test

build/main: $(DEPS)
	mkdir -p build
	$(CC) $(CFLAGS) -o build/main src/main.c

build/test: $(DEPS)
	mkdir -p build
	$(CC) $(CFLAGS) -DTEST -o build/test src/main.c

clean:
	rm -rf build
