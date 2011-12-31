
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Data.h"
#include "Defs.h"

extern int errno;
extern int debug;


field * setField(char *v)
{
	field *f;
	char *tok;

	if ((f = (field *)malloc(sizeof(struct Field))) == NULL) {
		fprintf(stderr, "setField: memory allocation error: %s\n", strerror(errno));
		exit(-1);
	}

	/* Field Identifier	*/
	if ((tok = strtok(v, "-")) != NULL)
		strcpy(f->ident, tok);
	else
		fprintf(stderr, "Warning: cannot get field-ident from %s\n", v);

	/* Field Length	*/
	if ((tok = strtok(NULL, "-")) != NULL)
		f->len = (u_short)atoi(tok);
	else
		fprintf(stderr, "Warning: cannot get field-length from %s\n", v);

	/* Field Start bit	*/
	if ((tok = strtok(NULL, "-")) != NULL)
		f->start = (u_short)atoi(tok);
	else
		fprintf(stderr, "Warning: cannot get field start bit  from %s\n", v);

	/* is next protocol identifier? */
	if ((tok = strtok(NULL, "-")) != NULL)
		f->protident = (u_short)atoi(tok);
	else
		fprintf(stderr, "Warning: cannot get field protident from %s\n", v);

	/* will inet_ntoa() be applied?	*/
	if ((tok = strtok(NULL, "-")) != NULL)
		f->inet_ntoa = (u_short)atoi(tok);
	else
		fprintf(stderr, "Warning: cannot get field inet_ntoa from %s\n", v);

	f->next = NULL;

	return f;
}


void addField(prot *p, field *f)
{
	field *ptr;

	if (p->fields == NULL) {
		p->fields = f;
		return;
	}

	for (ptr = p->fields; ptr->next != NULL; ptr = ptr->next)
		;

	ptr->next = f;
}


void listFields(prot *p)
{
	field *ptr;

	for (ptr = p->fields; ptr->next != NULL; ptr = ptr->next)
		printf("%s, %d bits, starts at bit %d, protident %d, inet_ntoa %d, ntohs %d\n", 
				ptr->ident, 
				ptr->len,
				ptr->start,
				ptr->protident,
				ptr->inet_ntoa,
				ptr->ntohs);
}
