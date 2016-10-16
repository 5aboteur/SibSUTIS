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

#define NPACKS  100

void dumperr(char *str)
{
    perror(str);
    exit(1);
}

char *rand_str_gen(char *str, int size)
{
	int i;
	const char charset[] = "abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"0123456789,.?@#$%^&*()[]{}|";

	if (size--) {
		for (i = 0; i < size; ++i) {
			int key = rand() % (int)(sizeof(charset) - 1);
			str[i] = charset[key];
		}
		str[size] = '\0';

		return str;
	}
	else dumperr("rand_str_gen");

	return str;
}

char *rand_str_alloc(int size)
{
	char *s = malloc(size + 1);

	if (s) {
		rand_str_gen(s, size);
		return s;
	}
	else dumperr("rand_str_alloc");

	return s;
}

int main(int argc, char **argv)
{
	if (argc < 3)
		dumperr("arguments (client)");

	srand((unsigned)time(NULL));

	int i, nbytes, port, serv_len, sock;
	struct sockaddr_in serv_addr;

	serv_len = sizeof(serv_addr);
	port = atoi(argv[2]);

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		dumperr("socket");

	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
		sizeof(timeout)) < 0)
		dumperr("setsockopt");

	if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
		sizeof(timeout)) < 0)
		dumperr("setsockopt");

	bzero((char *) &serv_addr, serv_len);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (!(inet_aton(argv[1], &serv_addr.sin_addr)))
		dumperr("inet_aton");

	for (i = 0; i < NPACKS; ++i) {
		int f = 0, l = 222;
		char *ans = malloc(sizeof(char) + 1);
		char *msg = malloc(sizeof(char) + l);
		msg = rand_str_alloc(l);

		while (1) {
			printf("\033[33mSending packet\033[0m %3d\033[33m, size:\033[0m %3d\n\033[33m"
				"Data:\033[0m %s\n\n", i + 1, l - 1, msg);

			if (sendto(sock, msg, l, 0, (struct sockaddr *) &serv_addr, serv_len) < 0)
				dumperr("sendto (client)");

			if (nbytes = recvfrom(sock, ans, 1, 0, (struct sockaddr *) &serv_addr,
				(socklen_t *) &serv_len) > 0)
				break;
		}

		sleep(1);
	}

	close(sock);

	return 0;
}
