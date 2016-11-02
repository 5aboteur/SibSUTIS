#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
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

#define BUFSIZE 1024
#define MSG_NUM 2

#define MSG_STREAM 0
#define PPM_STREAM 1

#define MAX_STREAMS 32

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
	int listener, nbytes, optval, sock;

	struct hostent *clnt_host;
	struct sctp_sndrcvinfo sndrcv_info;

	pid_t p;

	/* Create a one2one SCTP socket */
	if ((listener = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0)
		dumperr("socket (server)");

	/* This just allows you to reuse the server after ctrl+c */
	optval = 1;
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval,
		sizeof(int)) < 0)
		dumperr("setsockopt 1 (server)");

	/* Initialize server`s address */
	struct sockaddr_in serv_addr;
	int serv_len = sizeof(serv_addr);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = 0;

	/* Bind the socket */
	if (bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		dumperr("bind (server)");

	/* Enable association flags to get an info from the sctp_sndrcvinfo structure */
	struct sctp_event_subscribe events;

	memset((void *) &events, 0, sizeof(events));
	events.sctp_data_io_event = 1;
	events.sctp_association_event = 1;

	if (setsockopt(listener, SOL_SCTP, SCTP_EVENTS, (const void *) &events,
		sizeof(events)) < 0)
		dumperr("setsockopt 2 (server)");

	/* Initialize msg stream parameters */
	struct sctp_initmsg initmsg;

	memset((void *) &initmsg, 0, sizeof(initmsg));
	initmsg.sinit_num_ostreams = MAX_STREAMS;
	initmsg.sinit_max_instreams = MAX_STREAMS;
	initmsg.sinit_max_attempts = MAX_STREAMS;

	if (setsockopt(listener, SOL_SCTP, SCTP_INITMSG, (const void *) &initmsg,
		sizeof(initmsg)) < 0)
		dumperr("setsockopt 2 (server)");

	/* Get server`s port */
	if (getsockname(listener, (struct sockaddr *) &serv_addr,
		(socklen_t *) &serv_len) < 0)
		dumperr("getsockname (server)");

	printf("\033[34mServer port:\033[0m %d\n", ntohs(serv_addr.sin_port));

	/* Get ready for connections (max 5) */
	if (listen(listener, 5) < 0)
		dumperr("listen (server)");

	signal(SIGCHLD, killproc);

	struct sockaddr_in clnt_addr;
	int clnt_len = sizeof(clnt_addr);

	while (1) {
		/* Accept one of them */
		if ((sock = accept(listener, (struct sockaddr *) &clnt_addr,
			(socklen_t *) &clnt_len)) < 0)
			dumperr("accept (server)");

		/* Take a hostname from the client */
		if ((clnt_host = gethostbyaddr((const char *) &clnt_addr.sin_addr.s_addr,
			sizeof(clnt_addr.sin_addr.s_addr), AF_INET)) == NULL)
			dumperr("gethostbyaddr (server)");

		switch (p = fork()) {
		case -1:
			dumperr("fork (server)");
			break;
		case 0:
			close (listener);

			printf("\033[34m*********************************************************\n");
			printf("Established connection with \033[0m%s \033[34m->\033[0m %s:%d\n",
				clnt_host->h_name, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
			printf("\033[34m*********************************************************\033[0m\n");

			printf("\033[34mReading data..\033[0m\n");

			/* Construct a file`s name, where data will be store */
			char *prefix = malloc(sizeof(char) + 6);
			char *hostname = malloc(sizeof(char) + 32);
			char *filename = malloc(sizeof(char) + 40);
			char *extension = malloc(sizeof(char) + 5);

			prefix = "clnt_";
			hostname = clnt_host->h_name;
			extension = ".ppm";

			memcpy(filename, prefix, strlen(prefix));
			memcpy(filename + strlen(prefix), hostname, strlen(hostname) + 1);
			memcpy(filename + strlen(prefix) + strlen(hostname), extension,
				strlen(extension) + 1);

			FILE *fp;

			if (!(fp = fopen(filename, "ab")))
				dumperr("fopen (server)");

			char ans[20];
			if (recv(sock, ans, 20, 0) < 0)
				dumperr("recv (server)");

			while (1) {
				char *buf = malloc(sizeof(char) * BUFSIZE);
				bzero(buf, BUFSIZE);

				if ((nbytes = sctp_recvmsg(sock, buf, BUFSIZE, (struct sockaddr *) &clnt_addr,
					(socklen_t *) &clnt_len, &sndrcv_info, 0)) > 0)
					printf("\033[34mBytes received\033[0m %d\n", nbytes);
				else
					break;

				if (sndrcv_info.sinfo_stream == MSG_STREAM) {
					printf("\033[34mCHANNEL:\033[0m %d (msg stream)\n"
						"MSG: %s\n", sndrcv_info.sinfo_stream, buf);
				}
				else if (sndrcv_info.sinfo_stream == PPM_STREAM) {
					printf("\033[34mCHANNEL:\033[0m %d (ppm stream)\n",
						sndrcv_info.sinfo_stream);

					if (fwrite(buf, sizeof(char), nbytes, fp) < 0)
							dumperr("fwrite (server)");
				}
				else
					dumperr("sndrcv_info (server)");

				free(buf);
				printf("\033[34m\nWaiting for a data..\033[0m\n\n");
			}

			printf("\033[32mConnection closed!\n"
				"\033[34mPicture stored in \033[0m%s\n", filename);

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
