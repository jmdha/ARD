PROJECT_NAME   = ARD
PROJECT_AUTHOR = Jan M. D. Hansen
CC             = gcc
WFLAGS         = -Wall -Wextra -Wshadow
CFLAGS         = -O3 -march=native -ggdb

all: 
	$(CC) $(WFLAGS) $(CFLAGS) -o $(PROJECT_NAME) main.c -lm
