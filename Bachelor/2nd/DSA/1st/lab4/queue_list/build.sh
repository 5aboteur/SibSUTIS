#!/bin/sh

gcc -Wall -c -o llist.o llist.c
gcc -Wall -o queue_llist llist.o queue_llist.c
