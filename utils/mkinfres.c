/* $Id$

	Tool to create the _heaplen resource

	Useage: MKINFRES resfile mapfile

*/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "../command.h"
#include "../res.h"
#include "../infores.h"

#define pch(ch) (isprint((unsigned)(ch) & 0xFF)? (unsigned)(ch) & 0xFF: '.')

FILE *txt = 0;
FILE *info = 0;
char buf[3*1024];

#include "../res_w.c"		/* Make this a single file project */

int scanMapFile(const char * const fnam
	, unsigned long * const pos
	, unsigned *extraSpace)
{	FILE *map;
	char *w1, *w2, *w3, *w4, *w5;

	assert(pos);
	assert(fnam);
	assert(extraSpace);

	*extraSpace = 0;

	if((map = fopen(fnam, "rt")) == 0) {
		printf("Failed to open file: %s\n", fnam);
		return 40;
	}
	while(fgets(buf, sizeof(buf), map)) {
		if((w1 = strtok(buf, " \t\r\n")) != 0
		 && (w2 = strtok(0, " \t\r\n")) != 0) {
		 	if((w3 = strtok(0, " \t\r\n")) == 0) {
				if(strcmp(w2, "__heaplen") == 0) {
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
			} else if((w4 = strtok(0, " \t\r\n")) != 0
			 && (w5 = strtok(0, " \t\r\n")) != 0) {
				if(strcmp(w5, "BSS") == 0
				 || strcmp(w5, "STACK") == 0) {
				 	unsigned long n;
				 	char *p = 0;

				 	n = strtoul(w3, &p, 16);
				 	if(p && *p == 'H') {
				 		if(n > UINT_MAX || (n += *extraSpace) > UINT_MAX) {
				 			puts("Extra space exceeds range");
				 			return 56;
				 		}
				 		*extraSpace = (unsigned)n;
				 	}
				}
			}
		}
	}

	fclose(map);
	printf("No valid entry of _heaplen found in: %s\n", fnam);
	return 50;
}

int addImageDisplacement(const char * const fnam
	, unsigned long * const pos
	, unsigned *extraSpace)
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
	if(exe.extraMin > *extraSpace)
		*extraSpace = exe.extraMin;
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

	if(txt) {
		switch(ttype) {
		case INFO_END:         fputs("End             ", txt); break;
		case INFO_EXTRA_SPACE: fputs("Extra space     ", txt); break;
		case INFO_POS_HEAPLEN: fputs("Heaplen position", txt); break;
		case INFO_ALIASES:     fputs("Alias size      ", txt); break;
		case INFO_HISTORY:     fputs("History size    ", txt); break;
		case INFO_DIRSTACK:    fputs("Dirstack size   ", txt); break;
		case INFO_BUFSIZE:     fputs("Bufsize         ", txt); break;
		default:               fputs("Unknown tag     ", txt); break;
		}
		fprintf(txt, ": %u", tlength);
		switch(tlength) {
		case 0: break;
		case 1:
			{	unsigned char ch;

				ch = *(unsigned char*)tvalue;
				fprintf(txt, ": %u 0x%02x (%c)", ch, ch, pch(ch));
			}
			break;
		case 2:
			{	unsigned w;

				w = *(unsigned*)tvalue;
				fprintf(txt, ": %5u 0x%04x", w, w);
			}
			break;
		case 4:
			{	unsigned long l;

				l = *(unsigned long*)tvalue;
				fprintf(txt, ": %lu 0x%08lx", l, l);
			}
			break;
		default:	break;
		}
		putc('\n', txt);
	}
}

void dumpTagU(int ttype, unsigned tvalue)
{	dumpTag(ttype, 2, &tvalue);
}

main(int argc, char **argv)
{	char *txtFile = 0;
	unsigned long heapPos;
	int rc;
	unsigned extraSpace;

	if(argc > 1 && *argv[1] == '/' && toupper(argv[1][1]) == 'T') {
		txtFile = &argv[1][2];
		--argc;
		++argv;
	}
	if(argc != 4) {
		puts("Useage: INFORES resfile mapfile exefile");
		return 127;
	}
	if((rc = scanMapFile(argv[2], &heapPos, &extraSpace)) != 0)
		return rc;
	if((rc = addImageDisplacement(argv[3], &heapPos, &extraSpace)) != 0)
		return rc;

	if(txtFile && (txt = fopen(txtFile, "wt")) == 0) {
		printf("Failed to create text file: %s\n", txtFile);
		return 43;
	}
	if((info = fopen(argv[1], "wb")) == 0) {
		printf("Failed to create file: %s\n", argv[1]);
		return 41;
	}
	startResource(info, RES_ID_INFO, 0);
	dumpTagU(INFO_EXTRA_SPACE, extraSpace);
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
