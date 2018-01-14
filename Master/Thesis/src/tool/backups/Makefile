CC := mpicc

objects := gnuplot_i.o plot.o

.PHONY : all clean

all: gnuplot plot make move

gnuplot:
	$(CC) -c gnuplot_i.c -o gnuplot_i.o

plot:
	$(CC) -c plot.c -o plot.o

make:
	ar rcs libplot.a $(objects)
	ranlib libplot.a

move:
	mv libplot.a ../lib

clean:
	rm -f *.o
