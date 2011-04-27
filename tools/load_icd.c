/* $Id$

	Load Installable Commands samples into memory
 */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"

#include "portable.h"
#ifdef _TC_EARLY_
# include "fmemory.h"
#endif

#define BUFFER_SIZE 10*1024
const char id[] = "INSTALLABLE COMMAND - ICD v1.0";

main(int argc, char **argv)
{	unsigned char *buf, *h;
	size_t len, offName, offPtr;
	unsigned *p;
	FILE *f;
	char *q;
	IREGS r;
	unsigned char _seg *icmd;

	if(argc != 2) {
		puts("Load Installable Commands (FreeCOM shell extensions)");
		fputs("\nUseage: ", stdout);
		fputs(argv[0], stdout);
		puts(" command");
		return 127;
	}

	if((buf = malloc(BUFFER_SIZE)) == NULL) {
		puts("Cannot allocate transfer buffer - Out of memory");
		return 33;
	}

	if((f = fopen(argv[1], "rb")) == NULL) {
		strcpy(stpcpy((char*)buf, argv[1]), ".icd");
		if((f = fopen(buf, "rb")) == NULL) {
			fputs("Cannot open file: ", stdout);
			puts(argv[1]);
			return 34;
		}
	}

	len = fread(buf, 1, BUFFER_SIZE, f);
	if(ferror(f) || len == (size_t)-1 || len < sizeof(id) + 4) {
		fputs("Error reading command: ", stdout);
		puts(argv[1]);
		return 35;
	}
	fclose(f);
	if(len == BUFFER_SIZE) {
		fputs("Command too large: ", stdout);
		puts(argv[1]);
		return 36;
	}

	h = &buf[len - (sizeof(id) - 1)];
	if(memcmp(h, id, sizeof(id) - 1)) {
		puts("File does not contain a valid Installable Command");
		return 37;
	}

	*h = 0;
	while(--h > buf && *h);
	if(h == buf) {
		puts("File does not contain a valid Installable Command");
		return 39;
	}
	p = (unsigned*)h;
	offName = *--p;
	offPtr  = *--p;

	if(*++h) {
		puts("Installable Command comment record:");
		puts((char*)h);
	}

	if(offName > len - sizeof(id) + 1 || offPtr > len - sizeof(id) + 1) {
		puts("File does not contain a valid Installable Command");
		return 38;
	}

	/* Add Installable Command name */
	for(q = strchr(argv[1], 0); --q >= argv[1] && *q != ':' && *q != '\\'; )
		if(*q == '.')
			*q = 0;
	++q;
	if(!strlen(q) || strlen(q) > sizeof(id) - 2) {
		puts("Invalid filename syntax");
		return 40;
	}

	strupr(q);
	buf[offName] = (unsigned char)strlen(q);
	strcpy((char*)&buf[offName + 1], q);
	len = offName + strlen(q) + 2;

	r.r_ax = 0x352f;
	intrpt(0x21, &r);
	*(void far**)&buf[offPtr] = MK_FP(r.r_es, r.r_bx);

	/* Add into memory */
	r.r_ax = 0x4800;
	r.r_bx = (len + 15) / 16;
	intrpt(0x21, &r);
	if(r.r_flags & 1) {
		puts("Error allocating memory for resident portion of command");
		return 41;
	}

	icmd = MK_SEG_PTR (unsigned char, r.r_ax - 1);
	*(unsigned far *)&icmd[1] = 8;		/* make it system property */
	_fmemcpy(&icmd[8], &buf[offName + 1], 8);
	_fmemcpy(&icmd[16], buf, len);

	r.r_ds = r.r_ax;
	r.r_dx = 0;
	r.r_ax = 0x252f;
	intrpt(0x21, &r);

	printf("Installable Command %s loaded to 0x%04x\n"
	 , &buf[offName + 1], r.r_ds);

	return 0;
}
