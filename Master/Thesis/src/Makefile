EXEC=test_mpi_t
CC=mpicc

all: clean $(EXEC) run

$(EXEC):
	$(CC) $(EXEC).c -o $(EXEC).out

run:
	mpirun -n 2 ./$(EXEC).out

clean:
	rm -f *.out
