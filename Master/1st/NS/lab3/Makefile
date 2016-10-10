CC=gcc
CFLAGS=-Wall
SCTP=-lsctp

all: client server

client: client.c
	$(CC) $(CFLAGS) client.c -o client $(SCTP)

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(SCTP)

clean:
	rm -f client server clnt_*
