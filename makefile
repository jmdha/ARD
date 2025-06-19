PROJECT_NAME   = ard
PROJECT_AUTHOR = Jan M. D. Hansen
CC             = gcc
WFLAGS         = -Wall -Wextra -Wshadow
CFLAGS         = -O0 -march=native -ggdb

all: 
	$(CC) $(WFLAGS) $(CFLAGS) -o ard main.c -lm
