/* $Id$

	Tool to create the _heaplen resource

	Useage: MKINFRES resfile mapfile

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "../command.h"
#include "../res.h"
#include "../infores.h"

#include <algnbyte.h>
struct EXE_header {
	unsigned sig, lastFill, numBlocks, numReloc;
	unsigned header;
	unsigned extraMin, extraMax;
	unsigned fSS, fSP, checksum, fIP, fCS;
	unsigned offReloc, ovrlyNum;
};
#include <algndflt.h>


FILE *info = 0;
char buf[3*1024];

#include "../res_w.c"		/* Make this a single file project */

int getHeaplenPos(const char * const fnam, unsigned long * const pos)
{	FILE *map;
	char *w1, *w2;

	assert(pos);
	assert(fnam);

	if((map = fopen(fnam, "rt")) == 0) {
		printf("Failed to open file: %s\n", fnam);
		return 40;
	}
	while(fgets(buf, sizeof(buf), map)) {
		if((w1 = strtok(buf, " \t\r\n")) != 0
		 && (w2 = strtok(0, " \t\r\n")) != 0
		 && strtok(0, " \t\r\n") == 0
		 && strcmp(w2, "__heaplen") == 0) {
			unsigned long n1, n2;

			n1 = strtoul(w1, &w1, 16);
			if(w1 && *w1 == ':') {
				n2 = strtoul(w1 + 1, &w1, 16);
				if(w1 == 0 || *w1 == 0) {
					*pos = (unsigned long)n1 * 16 + n2;
					fclose(map);
					return 0;
				}
			}
		}
	}

	fclose(map);
	printf("No valid entry of _heaplen found in: %s\n", fnam);
	return 50;
}

int addImageDisplacement(const char * const fnam, unsigned long * const pos)
{	FILE *f;
	struct EXE_header exe;

	assert(fnam);
	assert(pos);

	if((f = fopen(fnam, "rb")) == 0) {
		printf("Failed to open file: %s\n", fnam);
		return 42;
	}
	if(fread(&exe, sizeof(exe), 1, f) != 1) {
		printf("Read error from: %s\n", fnam);
		return 52;
	}
	fclose(f);
	*pos += exe.header * 16;
	return 0;
}

void dumpTag(int ttype, int tlength, void *tvalue)
{
	infotag tag;
	assert(sizeof(tag) == 2);
	assert(info);

	tag.type = ttype;
	tag.length = tlength;
	fwrite(&tag, sizeof(tag), 1, info);
	if(tlength) {
		assert(tvalue);
		fwrite(tvalue, tlength, 1, info);
	}
}

void dumpTagU(int ttype, unsigned tvalue)
{	dumpTag(ttype, 2, &tvalue);
}

main(int argc, char **argv)
{
	unsigned long heapPos;
	int rc;

	if(argc != 4) {
		puts("Useage: INFORES resfile mapfile exefile");
		return 127;
	}
	if((rc = getHeaplenPos(argv[2], &heapPos)) != 0)
		return rc;
	if((rc = addImageDisplacement(argv[3], &heapPos)) != 0)
		return rc;

	if((info = fopen(argv[1], "wb")) == 0) {
		printf("Failed to create file: %s\n", argv[1]);
		return 41;
	}
	startResource(info, RES_ID_INFO, 0);
	dumpTag(INFO_POS_HEAPLEN, 4, &heapPos);
#ifdef FEATURE_ALIASES
	dumpTagU(INFO_ALIASES, 0);
#endif
#ifdef FEATURE_HISTORY
	dumpTagU(INFO_HISTORY, 256);
#endif
#ifdef INCLUDE_CMD_PUSHD
	dumpTagU(INFO_DIRSTACK, DIR_STACK_LEN);
#endif
	dumpTagU(INFO_BUFSIZE, MAX_INTERNAL_COMMAND_SIZE);
	dumpTag(INFO_END, 0, 0);
	endResource(info);

	fflush(info);
	if(ferror(info)) {
		printf("Error writing file: %s\n", argv[1]);
		return 60;
	}

	fclose(info);
	return 0;
}
