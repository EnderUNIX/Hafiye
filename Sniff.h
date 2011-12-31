#ifndef SNIFF_H
#define SNIFF_H

#include <pcap.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>



pcap_t	*pd;		/* Pcap Descriptor	*/
char *	dev;		/* Sniff Device		*/
char errbuf[PCAP_ERRBUF_SIZE];

bpf_u_int32	netp;
bpf_u_int32	maskp;
struct bpf_program	fprog;	/* Filter Program	*/

void startSniffer(char *);
void startSniffLoop();



#endif
