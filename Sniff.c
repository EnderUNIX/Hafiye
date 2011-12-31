
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

#include "Sniff.h"
#include "Packet.h"
#include "Defs.h"

extern bpf_u_int32 netp;
extern bpf_u_int32 maskp;
extern int datalink;
extern int promisc;
extern int readtimeout;
extern int npackets;

void startSniffer(char *filter)
{
	struct in_addr in;

	if (dev == NULL)
		if ((dev = pcap_lookupdev(errbuf)) == NULL) {
			fprintf(stderr, "startSniffer: couldn't find a device to sniff: %s\n", errbuf);
			exit(1);
		}

	printf("\n\n%s starting: listening on device: %s\n", VERSION, dev);

	if ((pd = pcap_open_live(dev, SNAPLEN, promisc, readtimeout, errbuf)) == NULL) {
		fprintf(stderr, "startSniffer: pcap_open_live failed: %s\n", errbuf);
		exit(1);
	}


	pcap_lookupnet(dev, &netp, &maskp, errbuf);
	in.s_addr = netp;
	printf("%s (%s/", dev, inet_ntoa(in));
	in.s_addr = maskp;
	printf("%s) opened successfully in %spromiscuous mode\n", inet_ntoa(in), (promisc ? "" : "non-"));

	if (filter != NULL) {
		pcap_compile(pd, &fprog, filter, 0, netp);
		if ((pcap_setfilter(pd, &fprog)) == -1) {
			fprintf(stderr, "startSniffer: pcap_setfilter: cannot set filter\n");
			exit(1);
		}
		pcap_freecode(&fprog);
	}
}


void startSniffLoop()
{
	int datalink;

	datalink = pcap_datalink(pd);
	if (pcap_loop(pd, npackets, main_packet_handler, (u_char *)datalink) < 0) {
		fprintf(stderr, "startSniffer: pcap_loop: %s\n", pcap_geterr(pd));
		exit(1);
	}
}
