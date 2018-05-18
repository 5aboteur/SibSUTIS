name := libumqtr
objects := dump.o gnuplot_i.o plot.o prof.o

CC := mpicc
CFLAGS := -std=gnu99 -Wall -Wextra -pedantic -O2
SRC := src

all: dump gnuplot plot prof pack move clean

dump:
	$(CC) $(CFLAGS) -c $(SRC)/dump.c -o dump.o

gnuplot:
	$(CC) $(CFLAGS) -c $(SRC)/gnuplot_i.c -o gnuplot_i.o

plot:
	$(CC) $(CFLAGS) -c $(SRC)/plot.c -o plot.o

prof:
	$(CC) $(CFLAGS) -c $(SRC)/prof.c -o prof.o

pack:
	ar rcs $(name).a $(objects)
	ranlib $(name).a

move:
	mkdir -p lib
	mv $(name).a lib

clean:
	rm -f *.o
