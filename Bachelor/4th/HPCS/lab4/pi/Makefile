pi := pi
pi_objs := pi.o hpctimer.o

piomp := piomp
piomp_objs := piomp.o hpctimer.o

piomp_red := piomp_red
piomp_red_objs := piomp_red.o hpctimer.o

CC := gcc
CFLAGS := -O0 -Wall -g -fopenmp
LDFLAGS := -fopenmp

.PHONY: all clean

all: $(pi) $(piomp) $(piomp_red)

$(pi): $(pi_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

$(piomp): $(piomp_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

$(piomp_red): $(piomp_red_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

pi.o: pi.c 
piomp.o: piomp.c
piomp_red.o: piomp_red.c
hpctimer.o: hpctimer.c hpctimer.h

clean:
	@rm -rf *.o $(pi) $(piomp) $(piomp_red)
