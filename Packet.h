#ifndef PACKET_H
#define PACKET_H

#include <pcap.h>

#include <sys/types.h>

void main_packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
void layer_two_handler(int, int, const u_char *);
void layer_three_handler(int, int, const u_char *);
void layer_four_handler(int, int, const u_char *);

#endif
