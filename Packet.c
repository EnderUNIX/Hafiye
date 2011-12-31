
#include <pcap.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/if_ether.h>

#include "Packet.h"
#include "Data.h"
#include "Infra.h"


void main_packet_handler(u_char *userdata, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	int next;
	int datalink;


	printf("\nNew Packet %d bytes\n", pkthdr->len);	
	switch((datalink = (int)userdata)) {
		case DLT_EN10MB:		/* 10Mbit/sec Ethernet	*/
			next = getbits(ntohl(*(packet + 12)), 31, 16);
			layer_two_handler(pkthdr->len - 14, next, packet + LAYERONE_LEN);
			break;
		default:
			printf("main_packet_handler: datalink type %d is unknown,\n", datalink);
			printf("main_packet_handler: I'll not process the packet any further\n");
			break;
	}
}



void layer_two_handler(int len, int proto, const u_char *packet)
{
	prot *p;
	field *f;
	int *ap;
	int protident;
	struct in_addr in;
	unsigned retn;

	if (proto > MAXLAYER) {
		fprintf(stderr, "layer_two_packet_handler: Invalid Protocol in packet: (%d > MAXLAYER)\n", proto);
		return;
	}

	if((p = ltwo[proto]) == NULL) {
		fprintf(stderr, "layer_two_packet_handler: I don't have protocol knowledge base for Protocol ID: %d\nPlease add knowledge base information with Protocol_Id=%d to Layer II knowledge base\nand restart me!\n", proto, proto);
		return;
	}

	printf("Layer II - %s\n", p->ident);
	for (f = p->fields; f != NULL; f = f->next) {
		ap = (int *)(packet + ((f->start / 32) * sizeof(int)));
		retn = getbits(ntohl(*ap), (31 - (f->start % 32)), f->len);
		if (f->inet_ntoa) {
			in.s_addr = *ap;
			printf("%s --> %s\n", f->ident, inet_ntoa(in));
		} else
			printf("%s --> 0x%x\n", f->ident, retn);
		if (f->protident)
			protident = retn;
	}
	layer_three_handler(len - p->len, protident, (packet + p->len));

}

void layer_three_handler(int len, int proto, const u_char *packet)
{
	prot *p;
	field *f;
	int *ap;
	int protident = -1;
	struct in_addr in;
	unsigned retn;

	if (proto > MAXLAYER) {
		fprintf(stderr, "layer_three_packet_handler: Invalid Protocol in packet: (%d > MAXLAYER)\n", proto);
		return;
	}

	if((p = lthree[proto]) == NULL) {
		fprintf(stderr, "layer_three_packet_handler: I don't have protocol knowledge base for Protocol ID: %d\nPlease add knowledge base information with Protocol_Id=%d to Layer III knowledge base\nand restart me!\n", proto, proto);
		return;
	}

	printf("\tLayer III - %s\n", p->ident);
	for (f = p->fields; f != NULL; f = f->next) {
		ap = (int *)(packet + ((f->start / 32) * sizeof(int)));
		retn = getbits(ntohl(*ap), (31 - (f->start % 32)), f->len);
		if (f->inet_ntoa) {
			in.s_addr = *ap;
			printf("\t%s --> %s\n", f->ident, inet_ntoa(in));
		} else
			printf("\t%s --> 0x%x\n", f->ident, retn);
		if (f->protident && (f->protident < 1024))
			protident = retn;
	}
	layer_four_handler(len - p->len, protident, (packet + p->len));
}

void layer_four_handler(int len, int proto, const u_char *packet)
{
	prot *p;
	field *f;
	int *ap;
	int protident;
	struct in_addr in;
	unsigned retn = 0;
	int i = 0, j = 0;
	u_char *pptr = NULL;

	if (proto < 0)
		return;

	if (proto > MAXLAYER) {
		fprintf(stderr, "layer_four_packet_handler: Invalid Protocol in packet: (%d > MAXLAYER)\n", proto);
		return;
	}

	if((p = lfour[proto]) == NULL) {
		fprintf(stderr, "layer_four_packet_handler: I don't have protocol knowledge base for Protocol ID: %d\nPlease add knowledge base information with Protocol_Id=%d to Layer IV knowledge base\nand restart me!\n", proto, proto);
		fprintf(stderr, "Dumping packet contents...\n");
		for (i = 0; i < len; i++) {
			if (isgraph(packet[i]) || isspace(packet[i]))
				fputc(packet[i], stdout);
			else
				fputc('.', stdout);
		}
		fwrite("\n", sizeof(char), 1, stdout);
		fflush(stdout);
		return;
	}

	printf("\t\tLayer IV - %s\n", p->ident);
	for (f = p->fields; f != NULL; f = f->next) {
		ap = (int *)(packet + ((f->start / 32) * sizeof(int)));
		retn = getbits(ntohl(*ap), (31 - (f->start % 32)), f->len);
		if (f->inet_ntoa) {
			in.s_addr = *ap;
			printf("\t\t%s --> %s\n", f->ident, inet_ntoa(in));
		} else
			printf("\t\t%s --> 0x%x\n", f->ident, retn);
		if (f->protident > 0)
			protident = retn;
	}

	printf("\t\tPacket Payload Dump: \n");
	if (p != NULL) {
		pptr = packet + p->len;
		j = len - p->len;
		for (i = 0; i < j; i++) {
			if (isgraph(pptr[i]) || isspace(packet[i]))
				fputc(packet[i], stdout);
			else
				fputc('.', stdout);
		}
		fwrite("\n", sizeof(char), 1, stdout);
		fflush(stdout);
	}
}
