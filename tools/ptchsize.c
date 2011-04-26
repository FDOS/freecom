/* $Id$

	Patch heap size into FreeCOM executable

	Useage: PTCHSIZE freecom [{ size }]

	Without any argument: Display the internal information
	Else: patch the executable
		if the sum of "size" arguments evaluates to zero,
		the estimated minimum heap size is used.
*/

#undef DEBUG

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/res.h"
#include "../include/infores.h"


FILE *freecom = 0;
unsigned char *info = 0;
unsigned infoLen = 0;
unsigned long minSize;

struct {
	unsigned alias, hist, dirs, bufsize, extraSpace;
	unsigned long heapPos;
} ival;

#include "../lib/res_r.c"		/* make a single file project */

int getInfo (res_majorid_t major,
                    res_minorid_t minor,
                    unsigned long length,
                    FILE * f,
                    void *const arg) {
        (void)arg;
	if(major == RES_ID_INFO && minor == 0) {
		if(length >= INT_MAX) {
			puts("Info resource is too large");
			return 100;
		}
		if((info = malloc(infoLen = (unsigned)length)) == 0) {
			puts("Out of memory");
			return 101;
		}
		if(fread(info, (unsigned)length, 1, f) != 1) {
			puts("Read error from file");
			return 102;
		}
		return 1;		/* Success */
	}
	return 0;		/* Proceed */
}

#define err(num)	err_(i, (num))
void err_(int i, int num)
{	printf("Info resource corrupted at offset %u\n", i);
	exit(num);
}
void getInfoValues(void)
{	int i;
	unsigned *pu;

	ival.alias = ival.hist = ival.dirs = ~0;
	ival.extraSpace = 2 * 1024 / 16;
	ival.bufsize = 256;
	ival.heapPos = ~0;

	assert(sizeof(ival.heapPos) == 4);

	for(i = 0; i < infoLen; i += 1 + info[i]) {
		if((unsigned long)i + 1 + info[i + 1] > infoLen)
			err(81);
		switch(info[i++]) {
		case INFO_END:	return;
		case INFO_POS_HEAPLEN:
			if(info[i] != 4) err(82);
			memcpy(&ival.heapPos, &info[i + 1], 4);
			break;
		case INFO_ALIASES:
			pu = &ival.alias;
		valUnsigned:
			if(info[i] != 2) err(83);
			memcpy(pu, &info[i + 1], 2);
			break;
		case INFO_HISTORY:
			pu = &ival.hist;
			goto valUnsigned;
		case INFO_DIRSTACK:
			pu = &ival.dirs;
			goto valUnsigned;
		case INFO_BUFSIZE:
			pu = &ival.bufsize;
			goto valUnsigned;
		case INFO_EXTRA_SPACE:
			pu = &ival.extraSpace;
			goto valUnsigned;
		default:
			printf("Warning: Unknown info tag found: %u\n", info[i - 1]);
		}
	}
}

void prUns(unsigned val, char *name)
{
	fputs(name, stdout);
	if(val == (unsigned)~0)
		puts(" not included or in separated segment");
	else printf(" default buffer size: %u\n", val);
}

#define addUns(val)	addUns_(&minSize, (val))
void addUns_(unsigned long *minVal, unsigned val)
{	
	if(val != (unsigned)~0)
		*minVal += val;
}

void addSize(unsigned *size, char * const Xp)
{	unsigned long n;
	char *p = Xp;

	if(memcmp(p, "+", 2) == 0)
		n = minSize;
	else {
		n = 0;
		if(*p == '+')
			++p;
		if(*p == '0' && toupper(p[1]) == 'X') {
			/* parse hexadecimal */
			++p;
			while(isxdigit(*++p))
				if(isdigit(*p))
					n = n * 16 + *p - '0';
				else
					n = n * 16 + toupper(*p) - 'A' + 10;
		} else {
			/* parse decimal */
			while(isdigit(*p))
				n = n * 10 + *p++ - '0';
		}

		if(toupper(*p) == 'K') {
			n *= 1024;
			++p;
			if(toupper(*p) == 'B')
				++p;
		}

		if(*p) {
			printf("Number parse error: %s\n", Xp);
			exit(103);
		}
	}

	if(n > UINT_MAX || (n + *size) > UINT_MAX) {
		puts("Sum of sizes exceed 64KB");
		exit(104);
	}

	*size += (unsigned)n;
}

main(int argc, char **argv)
{	struct EXE_header exe;
	unsigned tosize;

	if(argc <= 1) {
		puts("Patch heap size into FreeCOM executable\n"
			"\nuseage: PTCHSIZE freecom [{ size }]\n"
			"\nSize may be given hexadecimal (0x###) or decimal.\n"
			"If 'KB' is appended to size, it's multiplied with 1024.\n"
			"A leading plus sign is ignored.\n"
			"A plus sign by its own is treated as estimated minimum size.\n"
			"More than one sizes are summed up.\n"
			"If size evaluates to 0 (zero), the restriction is disabled.\n"
			"\ne.g.: PTCHSIZE freecom + 5K\n"
			"means: reserve 5*1024 bytes in addition to minimum size."
		);
		return 127;
	}

	if(enumFileResources(argv[1], RES_ID_INFO, (int(*)())getInfo, 0) != 1) {
		puts("Failed to locate FreeCOM's info resource\n"
			"Possible errors:\n"
			"\tAn error is issued above.\n"
			"\tThe file could not be opened.\n"
			"\tThe version of FreeCOM is too old.\n"
			"\tFile read error.");
		return 80;
	}

	if((freecom = fopen(argv[1], "r+b")) == 0) {
		printf("Cannot open FreeCOM: %s\n", argv[1]);
		return 30;
	}

	if(fread(&exe, sizeof(exe), 1, freecom) != 1) {
		printf("Read error from: %s\n", argv[1]);
		return 44;
	}
	/* exe.extraMin is not trustworthy as it might have been tweak
		in a previous run */

	assert(info);
	/* Decompose the info resource */
	getInfoValues();
	minSize = 5 * (unsigned long)ival.bufsize	/* command lines */
		+ 3 * (64 + 3 + 8 + 5)				/* filenames */
		+ 256								/* env var buffer %PATH% etc */
		;
	addUns(ival.alias);
	addUns(ival.hist);
	addUns(ival.dirs);
	if(minSize > UINT_MAX) {
		puts("Estimated minimum heap size exceeds 64KB.\n"
			"There is most probably a corrupted info resource.");
		return 71;
	}

	if(argc == 2 || ival.heapPos == ~0) {
		prUns(ival.alias, "Aliases");
		prUns(ival.hist, "Command line history");
		prUns(ival.dirs, "Directory stack");
		prUns(ival.bufsize, "Command line buffer");
		if(ival.heapPos == ~0)
			puts("Missing heap position -> cannot change heap size");
		else {
			if(fseek(freecom, ival.heapPos, SEEK_SET) != 0) {
				printf("Failed to seek to heap size offset in %s\n", argv[1]);
				return 43;
			}
			assert(sizeof(tosize) == 2);
			if(fread(&tosize, sizeof(tosize), 1, freecom) != 1) {
				puts("Failed to read heap size from FreeCOM executable.");
				return 76;
			}
			fclose(freecom);
			if(tosize)
				printf("Heap size restricted to %u bytes\n"
				 , tosize);
			else
				puts("Heap size is currently not restricted");
		}
		printf("Estimated minimum of heap: %u\n", (unsigned)minSize);
		return 0;
	}

	tosize = 0;
	argc = 1;
	while(argv[++argc])
		addSize(&tosize, argv[argc]);
	if(tosize && tosize < minSize) {
		printf("size smaller than estimated minimum size: %u\n"
		 , (unsigned)minSize);
		return 72;
	}

	printf("Patching '%s' to heap size of %u bytes\n"
	 , argv[1], tosize);
	if(tosize)
		exe.extraMin = exe.extraMax = ival.extraSpace + tosize / 16;
	else {
		exe.extraMax = 0xffff;
		exe.extraMin = ival.extraSpace;
	}
	rewind(freecom);
	if(fwrite(&exe, sizeof(exe), 1, freecom) != 1) {
		printf("Failed to patch heap size into FreeCOM executable.\n"
			"File most probably corrupted now: %s\n", argv[1]);
		return 77;
	}

	if(fseek(freecom, ival.heapPos, SEEK_SET) != 0) {
		printf("Failed to seek to heap size offset in %s\n", argv[1]);
		return 42;
	}
	assert(sizeof(tosize) == 2);
	if(fwrite(&tosize, sizeof(tosize), 1, freecom) != 1) {
		printf("Failed to patch heap size into FreeCOM executable.\n"
			"File most probably corrupted now: %s\n", argv[1]);
		return 75;
	}

	fflush(freecom);
	if(ferror(freecom)) {
		printf("Failed to patch heap size into FreeCOM executable.\n"
			"File most probably corrupted now: %s\n", argv[1]);
		return 76;
	}
	fclose(freecom);
	return 0;
}
