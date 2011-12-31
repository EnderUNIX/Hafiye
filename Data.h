
#ifndef DATA_H
#define DATA_H


#include "Defs.h"

#include <sys/types.h>

typedef struct Field {
	char ident[BUFSIZE];
	u_short len;
	u_short start;
	u_char protident;
	u_char inet_ntoa;
	u_char ntohs;
	struct Field *next;
} field;


typedef struct Prot {
	int id;
	char ident[BUFSIZE];
	u_short	len;
	struct Field *fields;
} prot;


struct Prot *ltwo[MAXLAYER];		/* Layer II Protocols	*/
struct Prot *lthree[MAXLAYER];		/* Layer III Protocosl	*/
struct Prot *lfour[MAXLAYER];		/* Layer IV Protocols	*/


void addField(prot *, field *);
void listFields(prot *);
field * setField(char *);

#endif
