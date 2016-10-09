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

#define BUFSIZE 1024

void dumperr(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char **argv)
{
	if (argc < 4) {
		perror("arguments (client)");
		exit(1);
	}

	srand((unsigned)time(NULL));

	int file_len, port, serv_len, sock;
	struct sockaddr_in serv_addr;

	FILE *fp;

	serv_len = sizeof(serv_addr);
	file_len = 0;

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

	char *msg = malloc(sizeof(char) + BUFSIZE);

	if ((fp = fopen(argv[3], "rb")) == NULL)
		dumperr("fopen (client)");

	fseek(fp, 0, SEEK_END);

	if ((file_len = ftell(fp)) < 0)
		dumperr("ftell (client)");

	if (file_len > BUFSIZE)
		dumperr("file_len (client)");

	rewind(fp);

	if (fread(msg, sizeof(char), file_len, fp) != file_len)
		dumperr("fread (client)");

	printf("\033[33mSending message to server, size:\033[0m %3d\n\033[33m"
		"Data:\033[0m %s\n\n", file_len, msg);

	if (send(sock, msg, file_len, 0) < 0)
		dumperr("send (client)");

	fclose(fp);
	close(sock);

	return 0;
}
