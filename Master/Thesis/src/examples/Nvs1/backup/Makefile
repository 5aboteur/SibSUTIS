name := nvs1_umq_test
objects := nvs1_umq_test.o
lib_dir := ../../tool/lib

CC := mpicc
CFLAGS := -std=gnu99 -Wall -Wextra -Werror -pedantic -O2
EXEC := mpiexec
LDFLAGS := -lm -lpthread -L $(lib_dir) -lumqtr

all: name

name:
	$(CC) $(CFLAGS) $(name).c $(LDFLAGS) -o $(name)

clean:
	rm -f *.o gnuplot_tmpdatafile* $(name)

run:
	$(EXEC) -np 4 ./$(name) 100 --eager
