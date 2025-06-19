PROJECT_NAME   = ard
PROJECT_AUTHOR = Jan M. D. Hansen
CC             = gcc
WFLAGS         = -Wall -Wextra -Wshadow
CFLAGS         = -Isrc -Ithird_party -O2 -march=native -ggdb
SRC_DIR        = src

SRC            = $(shell find $(LIB_DIR) -type f -iname '*.c') 

all: 
	$(CC) $(WFLAGS) $(CFLAGS) -o ard $(SRC) -lm
