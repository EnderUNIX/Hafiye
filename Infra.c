
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "Data.h"
#include "Cfg.h"
#include "Infra.h"

extern int errno;
extern int debug;

/* Return n-bit field of x that begins at position p	
 * Kernighan & Ritchie (1988)
 */
unsigned getbits(unsigned x, int p, int n)
{
	return (x >> (p + 1 - n)) & ~(~0 << n);
}


void knowLayerTwo(char *dname)
{

	DIR *dir;
	struct dirent *dep;
	char *file;
	struct Prot *pr;


	file = (char *)calloc(1024, sizeof(char));

	if ((dir = opendir(dname)) == NULL) {
		fprintf(stderr, "Layer II Knowledge Base Directory %s failed: %s\n",
				dname, strerror(errno));
		exit(-1);
	}

	while((dep = readdir(dir)) != NULL) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		pr = (struct Prot *)malloc(sizeof(prot));
		sprintf(file, "%s/%s", dname, dep->d_name);
		if (debug)
			printf("Examining Layer II file %s...\n", dep->d_name);
		loadConfig(pr, file);
		ltwo[pr->id] = pr;
	}
	free(file);
}

void knowLayerThree(char *dname)
{

	DIR *dir;
	struct dirent *dep;
	char *file;
	struct Prot *pr;


	file = (char *)calloc(1024, sizeof(char));

	if ((dir = opendir(dname)) == NULL) {
		fprintf(stderr, "Layer III Knowledge Base Directory %s failed: %s\n",
				dname, strerror(errno));
		exit(-1);
	}

	while((dep = readdir(dir)) != NULL) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		pr = (struct Prot *)malloc(sizeof(prot));
		sprintf(file, "%s/%s", dname, dep->d_name);
		if (debug)
			printf("Examining Layer III file %s...\n", dep->d_name);
		loadConfig(pr, file);
		lthree[pr->id] = pr;
	}
	free(file);
}

void knowLayerFour(char *dname)
{
	DIR *dir;
	struct dirent *dep;
	char *file;
	struct Prot *pr;


	file = (char *)calloc(1024, sizeof(char));

	if ((dir = opendir(dname)) == NULL) {
		fprintf(stderr, "Layer IV Knowledge Base Directory %s failed: %s\n",
				dname, strerror(errno));
		exit(-1);
	}

	while((dep = readdir(dir)) != NULL) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		pr = (struct Prot *)malloc(sizeof(prot));
		sprintf(file, "%s/%s", dname, dep->d_name);
		if (debug)
			printf("Examining Layer IV file %s...\n", dep->d_name);
		loadConfig(pr, file);
		lfour[pr->id] = pr;
	}
	free(file);
}
