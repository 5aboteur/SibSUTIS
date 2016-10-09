#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 10000
#define NPACKS	100

#define MSG_STREAM 0
#define PPM_STREAM 1

void dumperr(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char **argv)
{
	if (argc < 5) {
		perror("arguments (client)");
		exit(1);
	}

	srand((unsigned)time(NULL));

	int i, file_len, port, serv_len, sock;
	struct sockaddr_in serv_addr;

	FILE *fp;

	serv_len = sizeof(serv_addr);
	file_len = 0;

	port = atoi(argv[2]);

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0)
		dumperr("socket (client)");

	bzero((char *) &serv_addr, serv_len);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (!(inet_aton(argv[1], &serv_addr.sin_addr)))
		dumperr("inet_aton (client)");

	if (connect(sock, (struct sockaddr *) &serv_addr, serv_len) < 0)
		dumperr("connect (client)");


	for (i = 0; i < NPACKS; ++i) {
		char *msg = malloc(sizeof(char) + BUFSIZE);

		if ((fp = fopen(argv[3], "rb")) == NULL)
			dumperr("fopen 1 (client)");

		fseek(fp, 0, SEEK_END);

		if ((file_len = ftell(fp)) < 0)
			dumperr("ftell 1 (client)");

		if (file_len > BUFSIZE)
			dumperr("file_len 1 (client)");

		rewind(fp);

		if (fread(msg, sizeof(char), file_len, fp) != file_len)
			dumperr("fread 1 (client)");

		printf("\033[33mSending message\033[0m %3d \033[33mto server,"
				"size:\033[0m %3d\n\033[33mData:\033[0m %s\n\n", i + 1, file_len, msg);

		if (sctp_sendmsg(sock, msg, file_len + 1, NULL, 0, 0, 0, MSG_STREAM, 0, 0) < 0)
			dumperr("sctp_sendmsg 1 (client)");

		fclose(fp);

		char *pic = malloc(sizeof(char) + BUFSIZE);

		if ((fp = fopen(argv[4], "rb")) == NULL)
			dumperr("fopen 2 (client)");

		fseek(fp, 0, SEEK_END);

		if ((file_len = ftell(fp)) < 0)
			dumperr("ftell 2 (client)");

		if (file_len > BUFSIZE)
			dumperr("file_len 2 (client)");

		rewind(fp);

		if (fread(pic, sizeof(char), file_len, fp) != file_len)
			dumperr("fread 2 (client)");

		printf("\033[33mSending picture to server, size:\033[0m %3d\n", file_len);

		if (sctp_sendmsg(sock, pic, file_len + 1, NULL, 0, 0, 0, PPM_STREAM, 0, 0) < 0)
			dumperr("sctp_sendmsg 2 (client)");

		fclose(fp);

		sleep(3);
	}

	close(sock);

	return 0;
}
