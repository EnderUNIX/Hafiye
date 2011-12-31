
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Data.h"
#include "Cfg.h"
#include "Infra.h"
#include "Sniff.h"
#include "main.h"

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int error = 0;
	int c;
	char *filter = NULL;

	dev = NULL;
	prefix = (char *)calloc(HBUFSIZE, sizeof(char));
	dir = (char *)calloc(BUFSIZE, sizeof(char));
	strcpy(prefix, "/usr/local/share/hafiye");

	while (!error && (c = getopt(argc,argv,"k:i:n:dhpv")) != -1) {
		switch(c) {
			case 'i':
				dev = (char *)calloc(DEVSIZ, sizeof(char));
				strncpy(dev, optarg, DEVSIZ);
				break;
			case 'k':
				strncpy(prefix, optarg, HBUFSIZE);
				break;
			case 'd':
				debug = 1;
				break;
			case 'n':
				npackets = atoi(optarg);
				break;
			case 'h':
				usage();
				exit(0);
				break;
			case 'p':
				promisc = 1;
				break;
			case 'v':
				printf("%s -- http://www.enderunix.org/hafiye/\n", VERSION);
				printf("by tarumnabalab@enderunix.org\n");
				exit(0);
				break;
			default:
				usage();
				error = 1;
				break;
		}
	}
	
	if (error)
		exit(1);

	if (argc > optind)
		filter = strdup(argv[optind]);


	snprintf(dir, BUFSIZE, "%s/%s", prefix, "LII");
	knowLayerTwo(dir);
	snprintf(dir, BUFSIZE, "%s/%s", prefix, "LIII");
	knowLayerThree(dir);
	snprintf(dir, BUFSIZE, "%s/%s", prefix, "LIV");
	knowLayerFour(dir);

	startSniffer(filter);
	startSniffLoop();

	return 0;
}


void usage()
{
	fprintf(stderr, "usage: hafiye [options] [\"pcap filter\"]\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr, "\t-i interface\t\t\tspecify interface to listen on\n");
	fprintf(stderr, "\t-k directory\t\t\tspecify location to knowledge-base files\n");
	fprintf(stderr, "\t-d\t\t\t\tshow debugging info\n");
	fprintf(stderr, "\t-p\t\t\t\tput into promiscuous mode\n");
	fprintf(stderr, "\t-n packet count\t\t\tnumber of packets to receive\n");
	fprintf(stderr, "\t-t timeout\t\t\tread timeout\n");
	fprintf(stderr, "\nhttp://www.enderunix.org/hafiye/\n");

}
