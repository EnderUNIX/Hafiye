OBJECTS = Data.o Infra.o Cfg.o Sniff.o Packet.o main.o
PCAPINC = /usr/include
PCAPLIB = /usr/lib
CFLAGS = -Wall -W -pedantic -I$(PCAPINC)
CC = cc
LDFLAGS = $(PCAPLIB)/libpcap.a


all: $(OBJECTS)
	$(CC) -o hafiye -g $(OBJECTS) $(LDFLAGS)
	
install:
	cp -f hafiye /usr/local/bin/
	mkdir -p /usr/local/share/hafiye
	cp -Rp KB/LII /usr/local/share/hafiye
	cp -Rp KB/LIII /usr/local/share/hafiye
	cp -Rp KB/LIV /usr/local/share/hafiye
cl:
	rm -f */*/*~

clean:
	rm -f *~ *.o hafiye core
