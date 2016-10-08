#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE	1024

void killproc()
{
	int wstatus = 0;
	wait(&wstatus);
}

void dumperr(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char **argv)
{
	char buf[BUFSIZE];
	int clnt_len, serv_len, listener, optval, sock;
	struct hostent *clnt_host;
	struct sockaddr_in serv_addr, clnt_addr;

	FILE *fp;

	pid_t p;

	signal(SIGCHLD, killproc);

	if ((listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		dumperr("socket (server)");

	optval = 1;
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval,
		sizeof(int)) < 0)
		dumperr("setsockopt (server)");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = 0;

	if (bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		dumperr("bind (server)");

	clnt_len = sizeof(clnt_addr);
	serv_len = sizeof(serv_addr);

	if (getsockname(listener, (struct sockaddr *) &serv_addr,
		(socklen_t *) &serv_len) < 0)
		dumperr("getsockname (server)");

	printf("\033[34mServer port:\033[0m %d\n", ntohs(serv_addr.sin_port));

	if (listen(listener, 5) < 0)
		dumperr("listen (server)");

	while (1) {
		if ((sock = accept(listener, (struct sockaddr *) &clnt_addr, (socklen_t *) &clnt_len)) < 0)
			dumperr("accept (server)");

		if ((clnt_host = gethostbyaddr((const char *) &clnt_addr.sin_addr.s_addr,
			sizeof(clnt_addr.sin_addr.s_addr), AF_INET)) == NULL)
			dumperr("gethostbyaddr (server)");

		switch (p = fork()) {
		case -1:
			dumperr("fork (server)");
			break;
		case 0:
			close (listener);

			printf("\033[34m****************************************************\n");
			printf("Established connection with \033[0m%s \033[34m->\033[0m %s:%d\n",
				clnt_host->h_name, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

			printf("\033[34mReading data..\033[0m\n");

			bzero(buf, BUFSIZE);

			if (recv(sock, buf, BUFSIZE, 0) < 0)
				dumperr("read (server)");

			int buflen = strlen(buf);

			char *prefix = malloc(sizeof(char) + 6);
			char *hostname = malloc(sizeof(char) + 32);
			char *filename = malloc(sizeof(char) + 40);

			prefix = "clnt_";
			hostname = clnt_host->h_name;

			memcpy(filename, prefix, strlen(prefix));
			memcpy(filename + strlen(prefix), hostname, strlen(hostname) + 1);

			if ((fp = fopen(filename, "ab")) == NULL)
				dumperr("fopen (server)");

			if (fwrite(buf, sizeof(char), buflen, fp) <= 0)
				dumperr("fwrite (server)");

			printf("\033[32mComplete!\n\033[34mData stored in \033[0mclnt_%s",
				clnt_host->h_name);

			printf("\n");
			printf("\033[32mTransmission succeed! Disconnect.\n");
			printf("\033[34m****************************************************\033[0m\n");

			fclose(fp);
			close(sock);
			exit(0);
		default:
			close (sock);
			break;
		}
	}

	return 0;
}
