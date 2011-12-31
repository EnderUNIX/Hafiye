#ifndef DEFS_H
#define DEFS_H


static const char VERSION[] = "EnderUNIX Hafiye Version 1.0";


enum {
	DEVSIZ = 16,
	SNAPLEN = 1514,
	PROMISC = 1,
	READTIMEOUT = 500,
	PACKET_COUNT = -1,	/* Infinite loop	*/
	LAYERONE_LEN = 14,
	BUFSIZE = 1024,
	HBUFSIZE = 512,
	KEYSIZE = 64,
	VALSIZE = 256,
	MAXLAYER = 5000
};


#endif
