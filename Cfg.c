#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Data.h"
#include "Cfg.h"
#include "Defs.h"

extern int errno;
extern int debug;


void loadConfig(struct Prot *p, char *file)
{
	FILE *fp;
	char buf[BUFSIZE];
	char keyword[KEYSIZE];
	char value[VALSIZE];
	char *cp1, *cp2;
	field *f;
	char *variables[] = { "Invalid",
			"Protocol_Id",
			"Protocol_Ident",
			"Protocol_Length",
			"Field" 
			};
	int i, key, line, nkeys;

	nkeys = sizeof(variables) / sizeof(char *);

	if ((fp = fopen(file, "r")) == NULL) {
		fprintf(stderr, "Cannot open knowledge-base file %s: %s\n", file, strerror(errno));
		exit(1);
	}
	line = 0;

	while ((fgets(buf, BUFSIZE, fp)) != NULL) {
		line++;
		if(buf[0] == '#')
			continue;
		cp1 = buf;
		cp2 = keyword;
		while(isspace((int)*cp1))
			cp1++;
		while(isgraph((int)*cp1) && (*cp1 != '='))
			*cp2++ = *cp1++;
		*cp2 = '\0';
		cp2 = value;

		while((*cp1 != '\0') && (*cp1 != '\n')
				&& (*cp1 != '='))
			cp1++;
		cp1++;
		
		while(isspace((int)*cp1))
			cp1++;
		
		if (*cp1 == '"')
			cp1++;
		
		while(*cp1 != '\0' && (*cp1 != '\n')
			&& (*cp1 != '#'))
			*cp2++ = *cp1++;
		*cp2-- = '\0';

		if (keyword[0] == '\0' || value[0] == '\0')
			continue;

		key = 0;

		for (i = 0; i < nkeys; i++) {
			if (strcmp(keyword, variables[i]) == 0) {
				key = i;
				break;
			}
		}

		switch(key) {
			case 0:
				printf("Invalid keyword \"%s\" in knowledge-base file %s, line %d\n",
						keyword, file, line);
				exit(-1);
				break;
			case 1:
				p->id = atoi(value);
				break;
			case 2:
				if (strlen(value) < BUFSIZE) 	/* strncpy weirdness!	*/
					strcpy(p->ident, value);
				break;
			case 3:
				p->len = atoi(value);
				break;
			case 4:
				f = setField(value);
				addField(p, f);
				break;
		}
	}
	if (strlen(p->ident) <= 0) {
		fprintf(stderr, "Protocol Identification not found in file %s\n", file);
		fprintf(stderr, "Either correct the file to have the proper form or if you think\n");
		fprintf(stderr, "this file is not a knowledge-base file, remove it!\n");
		fprintf(stderr, "\nExiting due to an irrecoverable failure...\n");
		exit(1);
	}
	if (debug)
		listFields(p);
	printf("Finished examining knowledge-base file %s Protocol Id: %d, Protocol Definition: %s\n", file, p->id, p->ident);
	fclose(fp);
}
