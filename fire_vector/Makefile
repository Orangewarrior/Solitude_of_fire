CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g -fstack-protector-strong -D_FORTIFY_SOURCE=2

all:
	mkdir -p bin_test
	$(CC) $(CFLAGS) src_test/main.c lib/fire_vector.c lib/mem_ops.c -o bin_test/fire_vector_test

clean:
	rm -rf *.o bin_test/*
