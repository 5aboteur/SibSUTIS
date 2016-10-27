histeq := hist
histeq_objs := hist.o

CC := gcc
CFLAGS := -std=c99 -g -Wall -O2 -fopenmp
LDFLAGS := -lm -fopenmp

.PHONY: all clean

all: $(histeq)

$(histeq): $(histeq_objs)
        $(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
        $(CC) $(CFLAGS) -c $< -o $@

hist.o: hist.c

clean:
        @rm -rf *.o $(histeq)
