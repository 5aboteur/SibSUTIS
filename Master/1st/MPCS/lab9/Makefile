EXEC=main
CC=gcc
CFLAGS=-std=gnu99 -Wall -g -pedantic
LIB=-lm

INC=-I inc

all: clean $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(INC) $(EXEC).c $(LIB)

clean:
	rm -f a.out
