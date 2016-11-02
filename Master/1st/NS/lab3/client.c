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

#define BUFSIZE 1024
#define NPACKS	100

#define MSG_STREAM 0
#define PPM_STREAM 1

#define MAX_STREAMS 32

void dumperr(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char **argv)
{
	if (argc < 5)
		dumperr("arguments (client)");

	int nbytes, port, sock;

	port = atoi(argv[2]);

	/* Create a one2one SCTP socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0)
		dumperr("socket (client)");

	/* Initialize server address */
	struct sockaddr_in serv_addr;
	int serv_len = sizeof(serv_addr);

	bzero((char *) &serv_addr, serv_len);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (!(inet_aton(argv[1], &serv_addr.sin_addr)))
		dumperr("inet_aton (client)");

	/* Enable association flags to get info from sctp_sndrcvinfo */
	struct sctp_event_subscribe events;

	memset((void *) &events, 0, sizeof(events));
	events.sctp_association_event = 1;
	events.sctp_data_io_event = 1;

	if (setsockopt(sock, SOL_SCTP, SCTP_EVENTS, &events, sizeof(events)) < 0)
		dumperr("setsockopt 1 (client)");

	/* Initialize msg stream parameters */
	struct sctp_initmsg initmsg;

	memset((void *) &initmsg, 0, sizeof(initmsg));
	initmsg.sinit_num_ostreams = MAX_STREAMS;
	initmsg.sinit_max_instreams = MAX_STREAMS;
	initmsg.sinit_max_attempts = MAX_STREAMS;

	if (setsockopt(sock, SOL_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg)) < 0)
		dumperr("setsockopt 2 (client)");

	/* Try to connect to the server */
	if (connect(sock, (struct sockaddr *) &serv_addr, serv_len) < 0)
		dumperr("connect (client)");

	printf("\033[33mConnected!\033[0m\n");

	while (1) {
		int choice;

		printf("\033[1;37m1. Send a handshake message to the server\n");
		printf("2. Send a file to the server\n");
		printf("3. Quit\n");
		printf(">\033[0m");
		scanf("%d", &choice);

		FILE *fp;

		switch (choice) {
		case 1:
			if (!(fp = fopen(argv[3], "rb")))
				dumperr("fopen 1 (client)");

			printf("\033[33mSending a handshake message to the server.\033[0m\n");

			while (1) {
				char *msg = malloc(sizeof(char) + BUFSIZE);

				if ((nbytes = fread(msg, sizeof(char), BUFSIZE, fp)) < 0)
					dumperr("fread 1 (client)");

				if (nbytes > 0) {
					if (sctp_sendmsg(sock, msg, nbytes, NULL, 0, 0, 0,
						MSG_STREAM, 0, 0) < 0)
						dumperr("sctp_sendmsg 1 (client)");
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
			break;
		case 2:
			if (!(fp = fopen(argv[4], "rb")))
				dumperr("fopen 2 (client)");

			printf("\033[33mSending a picture to the server.\033[0m\n");

			while (1) {
				char *pic = malloc(sizeof(char) + BUFSIZE);

				if ((nbytes = fread(pic, sizeof(char), BUFSIZE, fp)) < 0)
					dumperr("fread 2 (client)");

				if (nbytes > 0) {
					if (sctp_sendmsg(sock, pic, nbytes, NULL, 0, 0, 0,
						PPM_STREAM, 0, 0) < 0)
						dumperr("sctp_sendmsg 2 (client)");
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
			break;
		case 3:
			close(sock);
			exit(0);
		default:
			printf("Incorrect value! :(\n");
			break;
		}
	}

	return 0;
}
