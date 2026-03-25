CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g -fstack-protector-strong -D_FORTIFY_SOURCE=2
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LDFLAGS :=
else
LDFLAGS := -Wl,-z,relro,-z,now
endif

all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c lib/fire_narytree.c -o bin/test_tree $(LDFLAGS)

clean:
	rm -rf *.o bin/*
