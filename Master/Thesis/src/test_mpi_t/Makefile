EXEC=test_mpi_t
CC=mpicc

all: clean $(EXEC)

$(EXEC):
	$(CC) $(EXEC).c

run:
	mpirun -n 2 ./a.out

clean:
	rm -f *.out
