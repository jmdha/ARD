PROJECT_NAME   = ard
PROJECT_AUTHOR = Jan M. D. Hansen
CC             = gcc
WFLAGS         = -Wall -Wextra -Wshadow
CFLAGS         = -Isrc -Ithird_party -O3 -march=native -flto -ggdb
SRCS           = $(shell find src    -type f -iname '*.c' ! -iname 'main.c')
BENCHS         = $(shell find benchs -type f -iname '*.c' ! -iname 'main.c')
TESTS          = $(shell find tests  -type f -iname '*.c' ! -iname 'main.c')

.PHONY: all bench test

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o $(PROJECT_NAME) src/main.c    $(SRCS) -lSDL3 -lm

bench:
	$(CC) $(CFLAGS)           $(DEFINE) -o bench           benchs/main.c $(SRCS) $(BENCHS) -lSDL3 -lm
	./bench

test:
	$(CC) $(CFLAGS)           $(DEFINE) -o test            tests/main.c  $(SRCS) $(TESTS)  -lSDL3 -lm
	./test


clean:
	rm -f $(PROJECT_NAME)
