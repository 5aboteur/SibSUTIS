#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BLK "\033[1;30m"
#define RED "\033[1;31m"
#define GRN "\033[1;32m"
#define YLW "\033[1;33m"
#define BLU "\033[1;34m"
#define MAG "\033[1;35m"
#define CYN "\033[1;36m"
#define WHT "\033[1;37m"

FILE *logfile;

void dumperr(char *str)
{
	perror(str);
	exit(1);
}

void dumpinfo(const u_char *data, int size)
{
	int i, j;

	for (i = 0; i < size; ++i) {
		if (i && !(i % 16)) {
			fprintf(logfile, "		");
			for (j = i - 16; j < i; ++j) {
				if (data[j] >= 32 && data[j] <= 128)
					fprintf(logfile, "%c", (u_char) data[j]);
				else
					fprintf(logfile, ".");
			}
			fprintf(logfile, "\n");
		}

		if (!(i % 16)) fprintf(logfile, "	");

		fprintf(logfile, " %02X", (u_int) data[i]);

		if (i == size - 1) {
			for (j = 0; j < 15 - i % 16; ++j)
				fprintf(logfile, "    ");

			fprintf(logfile, "        ");

			for (j = i - i % 16; j <= i; ++j) {
				if (data[j] >= 32 && data[j] <= 128)
					fprintf(logfile, "%c", (u_char) data[j]);
				else
					fprintf(logfile, ".");
			}
			fprintf(logfile, "\n");
		}
	}
}

void print_icmp_packet(const u_char *buffer, int size)
{
	u_short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
	iphdrlen = iph->ihl * 4;

	struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
	int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof(icmph);

	fprintf(logfile, "\n\n ********* ICMP PACKET *********\n\n");
	fprintf(logfile, "ICMP Header:\n");
	fprintf(logfile, " > Type\t:\t%d", (u_int) icmph->type);
	switch (icmph->type) {
		case 0:
			fprintf(logfile, "  (ICMP Echo Request)\n");
			break;
		case 8:
			fprintf(logfile, "  (ICMP Echo Reply)\n");
			break;
		case 11:
			fprintf(logfile, "  (TTL Expired)\n");
			break;
		default:
			fprintf(logfile, "  (Other)\n");
			break;
	}
	fprintf(logfile, " > Code\t:\t%d\n", (u_int) icmph->code);
	fprintf(logfile, "\n\n +++++++++ DATA PAYLOAD +++++++++\n\n");
	dumpinfo(buffer + header_size, size - header_size);
	fprintf(logfile, "\n=============================================================================\n");

}

void print_tcp_packet(const u_char *buffer, int size)
{
	u_short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
	iphdrlen = iph->ihl * 4;

	struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
	int header_size = sizeof(struct ethhdr) + iphdrlen + tcph->doff * 4;

	fprintf(logfile, "\n\n ********* TCP PACKET *********\n\n");
	fprintf(logfile, "TCP Header:\n");
	fprintf(logfile, " > Src Port\t:\t%u\n", ntohs(tcph->source));
	fprintf(logfile, " > Dst Port\t:\t%u\n", ntohs(tcph->dest));
	fprintf(logfile, " > Hdr Len\t:\t%d bytes\n", (u_int) tcph->doff * 4);
	fprintf(logfile, "\n\n +++++++++ DATA PAYLOAD +++++++++\n\n");
	dumpinfo(buffer + header_size, size - header_size);
	fprintf(logfile, "\n=============================================================================\n");

}

void print_udp_packet(const u_char *buffer, int size)
{
	u_short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
	iphdrlen = iph->ihl * 4;

	struct udphdr *udph = (struct udphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
	int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof(udph);

	fprintf(logfile, "\n\n ********* UDP PACKET *********\n\n");
	fprintf(logfile, "UDP Header:\n");
	fprintf(logfile, " > Src Port\t:\t%u\n", ntohs(udph->source));
	fprintf(logfile, " > Dst Port\t:\t%u\n", ntohs(udph->dest));
	fprintf(logfile, " > UDP Len\t:\t%d\n",  ntohs(udph->len));
	fprintf(logfile, "\n\n +++++++++ DATA PAYLOAD +++++++++\n\n");
	dumpinfo(buffer + header_size, size - header_size);
	fprintf(logfile, "\n=============================================================================\n");

}

void process_packet(u_char *args, const struct pcap_pkthdr *pkth, const u_char *packet)
{
	static int icmp = 0, igmp = 0, tcp = 0, udp = 0, othp = 0, total = 0;

	int size = pkth->len;
	struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));

	++total;

	printf("\033[2J\033[1;1H");
	printf(MAG);
	printf("Proto number: ");
	printf(WHT);
	printf("%d\n", iph->protocol);

	switch (iph->protocol) {
	case 1:
		// icmp
		++icmp;
		printf(GRN);
		printf("ICMP packet arrived.\n");
		printf(WHT);
		print_icmp_packet(packet, size);
		break;
	case 2:
		// igmp
		++igmp;
		printf(GRN);
		printf("IGMP packet arrived.\n");
		printf(WHT);
		break;
	case 6:
		// tcp
		++tcp;
		printf(GRN);
		printf("TCP packet arrived.\n");
		printf(WHT);
		print_tcp_packet(packet, size);
		break;
	case 17:
		// udp
		++udp;
		printf(GRN);
		printf("UDP packet arrived.\n");
		printf(WHT);
		print_udp_packet(packet, size);
		break;
	default:
		// unknown protocol
		++othp;
		printf(RED);
		printf("Unknown packet arrived.\n");
		printf(WHT);
		break;
	}

	printf("+----------------------------------------------------+\n");
	printf("|  ICMP  |  IGMP  |  TCP  |  UDP  |  OTHP  |  TOTAL  |\n");
	printf("+----------------------------------------------------+\n");
	printf("|%8d|%8d|%7d|%7d|%8d|%9d|\n", icmp, igmp, tcp, udp, othp, total);
	printf("+----------------------------------------------------+\n");
}

int main(int argc, char **argv)
{
	int cnt = 1, i, j, n;
	char *devname, devs[100][100];
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_if_t	*alldevsp, *dev;
	pcap_t		*devhandler;

	const u_char *packet;
	struct pcap_pkthdr hdr;
	struct ether_header *eptr;
	struct bpf_program fp;

	bpf_u_int32 maskp;
	bpf_u_int32 netp;

	if ((logfile = fopen("log.txt", "w")) == NULL)
		dumperr("fopen");

	/*** PREPARATION STEP ***/

	printf("Finding available devices ... ");
	if (pcap_findalldevs(&alldevsp, errbuf))
		dumperr("pcap_findalldevs");
	printf("Done!\n");

	printf(YLW);
	printf("************ Devices ************\n");
	printf(WHT);
	for (dev = alldevsp; dev != NULL; dev = dev->next, cnt++) {
		printf(" %2d. %s - %s\n", cnt, dev->name, dev->description);

		if (dev->name != NULL)
			strcpy(devs[cnt], dev->name);
	}
	printf(YLW);
	printf("*********************************\n");
	printf(WHT);

	printf("Enter the number of device you want to sniff -> ");
	scanf("%d", &n);
	devname = devs[n];

	/*** START SNIFFING  ***/

	printf("Opening device %s for sniffing ... ", devname);
	if ((devhandler = pcap_open_live(devname, BUFSIZ, 1, 0, errbuf)) == NULL)
		dumperr("pcap_open_live");
	printf("Done!\n");

	pcap_loop(devhandler, 100, process_packet, NULL);

	/*** END IT WHEN GET 100 PACKS ***/

	pcap_close(devhandler);
	fclose(logfile);

	return 0;
} 
