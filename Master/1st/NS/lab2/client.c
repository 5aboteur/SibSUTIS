#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 256

void dumperr(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char **argv)
{
	if (argc < 4)
		dumperr("arguments (client)");

	int nbytes, port, serv_len, sock;
	struct sockaddr_in serv_addr;

	FILE *fp;

	serv_len = sizeof(serv_addr);

	port = atoi(argv[2]);

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		dumperr("socket (client)");

	bzero((char *) &serv_addr, serv_len);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (!(inet_aton(argv[1], &serv_addr.sin_addr)))
		dumperr("inet_aton (client)");

	if (connect(sock, (struct sockaddr *) &serv_addr, serv_len) < 0)
		dumperr("connect (client)");

	char ans[1];

	if (recv(sock, ans, 1, 0) < 0)
		dumperr("recv (client)");

	printf("\033[33mConnected!\033[0m\n");

	if (sendto(sock, argv[3], strlen(argv[3]), 0,
		(struct sockaddr *) &serv_addr, serv_len) < 0)
		dumperr("sendto (client)");

	if (!(fp = fopen(argv[3], "rb")))
		dumperr("fopen (client)");

	while (1) {
		char msg[BUFSIZE];
		bzero(msg, BUFSIZE);

		if ((nbytes = fread(msg, sizeof(char), BUFSIZE, fp)) < 0)
			dumperr("fread 1 (client)");

		if (nbytes > 0) {
			printf("\033[33mSending a message to the server,"
				"size:\033[0m %3d\n", nbytes);

			if (send(sock, msg, nbytes, 0) < 0)
				dumperr("send (client)");
		}

		if (nbytes < BUFSIZE) {
			if (feof(fp)) {
				printf("\033[33m.. Done!\033[0m\n");
				break;
			}

			if (ferror(fp))
				dumperr("fread 2 (client)");
		}
	}

	fclose(fp);
	close(sock);

	return 0;
}
