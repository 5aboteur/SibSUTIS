name := heat-2d
objects := heat-2d.o
lib_dir := ../tool/lib

CC := mpicc
CFLAGS := -std=gnu99 -Wall -pedantic -O2
EXEC := mpiexec
LDFLAGS := -lm -lpthread -L $(lib_dir) -lprof

all: name

name:
	$(CC) $(CFLAGS) $(name).c $(LDFLAGS) -o $(name)

clean:
	rm -f *.o $(name)

run:
	$(EXEC) -np 4 ./$(name) 1024 1024

