#ifndef MAIN_H
#define MAIN_H

int promisc = 0;	/* Promiscous flag Default = non 	*/
int npackets = -1 ;	/* Number of packets to receive		*/
int readtimeout = 500;	/* Read timeout				*/
int debug = 0;		/* Debug output				*/
char *prefix;		/* Knowledge base directory prefix	*/
char *dir;		


void usage();

#endif
