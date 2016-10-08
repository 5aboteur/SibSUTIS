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

#define BUFSIZE	222

void dumperr(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char **argv)
{
	char buf[BUFSIZE];
	int i, cnt, clnt_len, serv_len, nbytes, optval, sock;
	struct hostent *clnt_host;
	struct sockaddr_in serv_addr, clnt_addr;

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		dumperr("socket");

	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval,
		sizeof(int)) < 0)
		dumperr("setsockopt");
/*
	struct timeval timeout;
    timeout.tv_sec = 100;
    timeout.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
        sizeof(timeout)) < 0)
        dumperr("setsockopt");

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
        sizeof(timeout)) < 0)
        dumperr("setsockopt");
*/
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = 0;

	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		dumperr("bind");

	clnt_len = sizeof(clnt_addr);
	serv_len = sizeof(serv_addr);

	if (getsockname(sock, (struct sockaddr *) &serv_addr,
		(socklen_t *) &serv_len) < 0)
		dumperr("getsockname");

	printf("\033[34mServer port:\033[0m %d\n", ntohs(serv_addr.sin_port));

	cnt = 0;

	while (1) {
		++cnt;
		printf("\n\033[34mWaiting for data..\033[0m\n\n");
		fflush(stdout);

		bzero(buf, BUFSIZE);

		if ((nbytes = recvfrom(sock, buf, BUFSIZE, 0,
			(struct sockaddr *) &clnt_addr, (socklen_t *) &clnt_len)) < 0)
			dumperr("recvfrom (server)");

		if ((clnt_host = gethostbyaddr((const char *) &clnt_addr.sin_addr.s_addr,
			sizeof(clnt_addr.sin_addr.s_addr), AF_INET)) == NULL)
			dumperr("gethostbyaddr");

		printf("\033[34m****************************************************\n");

		printf("Received datagram from \033[0m%s \033[34m->\033[0m %s:%d\n",
			clnt_host->h_name, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

		int buflen = strlen(buf);

		printf("\033[34mData:\033[0m ");
		for (i = 0; i < buflen; ++i) {
			if (!(i % 45) && i)
				printf("\n      ");
			printf("%c", buf[i]);
		}

		printf("\n");

		if (!(cnt % 10)) {
			printf("\033[31mTransmission failed! Try again..\n");
//			printf("\033[31mSome data may lost!\n");

//			if ((nbytes = sendto(sock, "F", 1, 0,
//				(struct sockaddr *) &clnt_addr, clnt_len)) < 0)
//				dumperr("sendto (server)");
//			printf("NBYTES: %d\n", nbytes);
		}
		else {
			printf("\033[32mTransmission succeed!\n");
			if ((nbytes = sendto(sock, "G", 1, 0,
				(struct sockaddr *) &clnt_addr, clnt_len)) < 0)
				dumperr("sendto (server)");
//			printf("NBYTES: %d\n", nbytes);
		}

		printf("\033[34m****************************************************\033[0m\n");
	}

	return 0;
}
