PROJECT_NAME   = ard
PROJECT_AUTHOR = Jan M. D. Hansen
CC             = gcc
WFLAGS         = -Wall -Wextra -Wshadow
CFLAGS         = -Isrc -Ithird_party -O0 -march=native -ggdb
SRC_DIR        = src
LIB_DIR        = $(SRC_DIR)/lib
CLI_DIR        = $(SRC_DIR)/cli
GUI_DIR        = $(SRC_DIR)/gui

SRC_LIB        = $(shell find $(LIB_DIR) -type f -iname '*.c') 
SRC_CLI        = $(shell find $(CLI_DIR) -type f -iname '*.c') 
SRC_GUI        = $(shell find $(GUI_DIR) -type f -iname '*.c') 
SRC_BENCHS     = $(shell find benchs     -type f -iname '*.c') 
SRC_TESTS      = $(shell find tests      -type f -iname '*.c') 

.PHONY: test bench

all: cli gui

cli:
	$(CC) $(WFLAGS) $(CFLAGS) -o ardc  $(SRC_LIB) $(SRC_CLI) -lm

gui:
	$(CC) $(WFLAGS) $(CFLAGS) -o ardg  $(SRC_LIB) $(SRC_GUI) -lSDL3 -lm

bench:
	$(CC) $(WFLAGS) $(CFLAGS) -o bench $(SRC_LIB) $(SRC_BENCHS) -lm
	./bench

test:
	$(CC) $(WFLAGS) $(CFLAGS) -o test  $(SRC_LIB) $(SRC_TESTS) -lm
	./test
