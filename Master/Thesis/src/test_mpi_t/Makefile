name := test_mpi_t

CC := mpicc
CFLAGS := -std=gnu99 -Wall -pedantic -O2
EXEC := mpiexec

all: name

name:
	$(CC) $(CFLAGS) $(name).c -o $(name)

clean:
	rm -f *.o $(name)

run:
	$(EXEC) -np 2 ./$(name)
