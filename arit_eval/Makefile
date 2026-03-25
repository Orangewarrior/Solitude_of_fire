CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g -fstack-protector-strong -D_FORTIFY_SOURCE=2
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LDFLAGS :=
else
LDFLAGS := -Wl,-z,relro,-z,now
endif

SRC := src/AST.c src/arithmetic_eval.c src/main.c src/parser.c src/secure_operation.c

all:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o bin/arit_eval $(LDFLAGS)

clean:
	rm -f bin/arit_eval src/*.o *.o
