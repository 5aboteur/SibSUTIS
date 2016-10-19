prog := task1
prog_objs := task1.o

CC := gcc
CFLAGS := -std=c99 -Wall -O2 -mmmx -mfpmath=sse -msse -msse2 -msse3 -mavx
LDFLAGS := -lm

.PHONY: all clean

all: $(prog)

$(prog): $(prog_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

task1.o: task1.c

clean:
	@rm -rf *.o $(prog)
