/* $Id$

	Chunk a large dependency list into portions with less than
	128 entries each. They'll form a cascaded dependency list
	with "argv[1]" is the primary target.

*/

#include <stdio.h>
#include <string.h>

char buf[1024];

int main(int argc, char **argv)
{	int i;
	char *p;
	unsigned long lnr;
	int chunk;

	if(argc != 2) {
		fputs("Useage: CHUNK primary_target <infile >outfile\n", stderr);
		return 127;
	}

	fputs(argv[1], stdout);
	fputs(" : ", stdout);

	chunk = i = 0;
	lnr = 0;
	while(fgets(buf, sizeof(buf), stdin)) {
		++lnr;
		p = strchr(buf, '\0');
		if(*--p != '\n') {
			fprintf(stderr, "Line #%lu too long\n", lnr);
			return 40;
		}
		*p = 0;
		if(++i >= 127) {
			++chunk;
			printf(" \\\n\tchunk%u\n\nchunk%u : ", chunk, chunk);
			i = 0;
		}
		fputs(" \\\n\t", stdout);
		fputs(buf, stdout);
	}
	puts("\n\n");

	return 0;
}
