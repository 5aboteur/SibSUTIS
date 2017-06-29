EXEC=main
CC=gcc
CFLAGS=-std=gnu99 -Wall -g -pedantic -Wno-pointer-sign
GTK2=`pkg-config --libs --cflags gtk+-2.0`

INCDIR=inc
SRCDIR=src

SOURCES=$(wildcard $(SRCDIR)/*.c)
INCLUDES=-I $(INCDIR)

all: clean $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(INCLUDES) $(EXEC).c $(SOURCES) $(GTK2)

clean:
	rm -f *.out
