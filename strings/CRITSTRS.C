/*
This program will read in the language file for Critical Error
	strings and create the file:

strings.err - contains all the strings. will be appended on the end
              of the exe file.

There are two input files:
DEFAULT.ERR and the language file passed as argument to CRITSTRS.
Any definition in the latter will override setting from the
first one.

1) If an individual *.ERR file does not define a certain string, its
	contents is taken from the DEFAULT file.
2) The strings do not have any particular order within the *.ERR files,
	the order is implicitly applied by CRITSTRS.

The area is constructed in memory and dumped into the file after all
files have been read and the contents has passed all transformation
and validation checks.
Therefore it is recommended to build the program in the Compact
memory model.

2000/07/11 ska
started

26.03.2003 Dejan Strbac (strbac@phreaker.net) - hexadecimal bug

*/

#define MODULE_VERSION 0

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*ska: no resource in this project! #include "../resource.h"*/

#define fDAT "STRINGS.ERR"
#define fTXT "DEFAULT.ERR"
#define fEXT ".ERR"


#define MAXSTRINGS       256

typedef struct {
	char *name;				/* name of string */
	char *text;				/* text of this string */
} strings;

typedef unsigned short word;		/* MUST BE EXACTLY unsigned 16bit */
typedef unsigned char byte;			/* MUST BE EXACTLY unsigned 8bit */
#if sizeof(word) != 2
#error "word must be exactly an unsigned 16-bit value"
#endif
#if sizeof(byte) != 1
#error "byte must be exactly an unsigned 8-bit value"
#endif

strings strg[MAXSTRINGS] = {
	 { "READ", NULL }			/* 0 */
	,{ "WRITE", NULL }			/* 1 */
	,{ "BLOCK_DEVICE", NULL }	/* 2 */
	,{ "CHAR_DEVICE", NULL }	/* 3 */
	,{ "DOS", NULL }			/* 4 */
	,{ "FAT", NULL }			/* 5 */
	,{ "ROOT", NULL }			/* 6 */
	,{ "DATA", NULL }			/* 7 */
	,{ "IGNORE", NULL }			/* 8 */
	,{ "RETRY", NULL }			/* 9 */
	,{ "ABORT", NULL }			/* 10 */
	,{ "FAIL", NULL }			/* 11 */
	,{ "QUESTION", NULL }		/* 12 */
	,{ "DELIMITER", NULL }		/* 13 */
	,{ NULL, NULL }
};

int maxNr = -1;

#define STR_KEYS 14				/* combined keys */
#define STR_ERROR 15			/* first error string */

strings special[] = {
	 { "KEYS_IGNORE", NULL }
	,{ "KEYS_RETRY", NULL }
	,{ "KEYS_ABORT", NULL }
	,{ "KEYS_FAIL", NULL }
	,{ "UNKNOWN", NULL }
	,{ NULL, NULL }
};

#if sizeof(char*) != 4
#error "This program must be compiled with far data pointers"
#endif

char temp[256];

	/* keep it a single-file project */
/*ska: no resource in this project! #include "../res_w.c"*/

#define join(s1,s2)	strcpy(stpcpy(temp, s1), s2);
void pxerror(char *msg1, char *msg2)
{	join(msg1, msg2);
	perror(temp);
}

int tonum(int ch)
{	return isdigit(ch)? ch - '0'
	 : toupper(ch) - 'A' + 10;
}

void savetext(strings s[], int i, const char * const line)
{	const char *p;
	char *q;

	if(i > maxNr)
		maxNr = i;

	free(s[i].text);
	if((q = s[i].text = malloc(strlen(p = line) + 1)) == NULL) {
		fputs("Out of memory\n", stderr);
		exit(85);
	}
	/* Check  and compact the string */
	while(isspace(*p)) ++p;
	while(*p) {
		if(*p == '%') {
			switch(*++p) {
			case '%': break;		/* expands to single '%' */
			case '\0': --p; break;	/* dito */
			case '.': 		/* ignore completely */
				++p;
				continue;
			case '&':		/* hexa-decimal string */
				if(isxdigit(p[1]) && isxdigit(p[2])) {
					*q++ = (tonum(p[1]) << 4) | tonum(p[2]);
					p += 3;
					continue;
				}
				/** fall though **/
			case '1': case '2': case '3': case 'A':
				/* Leave both in the string */
				*q++ = '%';
				break;
			}
		}
		*q++ = *p++;
	}
	*q++ = '\0';
}

int search(strings s[], const char * const k, const char * const line)
{	int i = 0;

	do if(strcmp(s[i].name, k) == 0) {
		savetext(s, i, line);
		return 1;
	} while(s[++i].name);

	return 0;
}

int loadFile(char *fnam)
{	unsigned long linenr;
	char *p;
	FILE *fin;

	join(fnam, fEXT);
	if((fin = fopen(fnam, "rt")) == NULL
	 && (fin = fopen(temp, "rt")) == NULL) {
		pxerror("opening ", fnam);
		return 33;
	}

	printf("CRITSTRS: load file %s\n", fnam);

	linenr = 0;
	while (fgets(temp, sizeof(temp), fin)) {
		++linenr;
		p = strchr(temp, '\0');
		if(p[-1] != '\n') {
			fprintf(stderr, "Line %lu too long\n", linenr);
			return 41;
		}
			/* Cut trailing control characters */
		while (--p >= temp && (iscntrl(*p) || isspace(*p)));
		p[1] = '\0';

		switch (*temp) {
		case ';': case '#': case '\0':	/* comment line */
			break;
		case 'S':		/* possible help line */
			if(isdigit(temp[1]))		/* help line */
				break;
		default:		/* line */
			if((p = strchr(temp, ':')) == NULL) {
				fprintf(stderr, "Syntax error in line %lu\n", linenr);
				return 42;
			}
			*p = '\0';
			if(isdigit(*temp)) {		/* error string */
				char *q = temp;
				int nr = 0;
				do nr = nr * 10 + *q - '0';
				while(isdigit(*++q));
				if(*q) {
					fprintf(stderr, "Invalid number in line %lu\n", linenr);
					return 44;
				}
				if(nr < 0 || nr > 255 - STR_ERROR) {
					fprintf(stderr, "String number too large in line %lu\n"
					 , linenr);
					return 45;
				}
				savetext(strg, STR_ERROR + nr, p + 1);
			} else {
				strupr(temp);
				if(!search(strg, temp, p + 1)
				 && !search(special, temp, p + 1)) {
					fprintf(stderr, "Unknown string name in line %lu\n"
					 , linenr);
					return 43;
				}
			}
		}
	}
	if(ferror(fin)) {
		pxerror("reading ", fnam);
		return 34;
	}
	fclose(fin);

	return 0;
}


int main(int argc, char **argv)
{
	FILE *dat;
	int rc, i, j;
	unsigned long size;

	/*word w;*/
	byte *p;
	/*byte b, *p;*/


	if(argc > 2) {
		puts("CRITSTRS - Generate Critical Error string for a language\n"
			"Useage: CRITSTRS [language]\n"
			"\tIf no language is specified, the default strings are read only.\n"
			"\tThere must be <language>.ERR file in the current directory.\n"
			"Note: DEFAULT.ERR must be present in the current directory, too.");
		return 127;
	}



	if((rc = loadFile(fTXT)) != 0)
		return rc;
	if(argc > 1 && (rc = loadFile(argv[1])) != 0)
		return rc;

/* Now all the strings are cached into memory */

	puts("CRITSTRS: Running validation check");

	i = rc = 0;
	do if(!strg[i].text) {
		fprintf(stderr, "Missing string \"%s\"\n", strg[i].name);
		rc = 46;
	} while(strg[++i].name);
	i = 0;
	do if(!special[i].text) {
		fprintf(stderr, "Missing string \"%s\"\n", special[i].name);
		rc = 47;
	} while(special[++i].name);
	if(rc) return rc;

	/* Create the compacted S14 (keys)
		format: LK..KA..A
			'L': number of K's
			'K': Keycode (== ASCII code)
			'A': Action code
			The number of keycodes is equal to the number of action codes.
			The leftmost keycode is associated to the leftmost action code.
	*/
		/* first: count */
	for(rc = i = 0; i < 4; ++i)
		rc += strlen(special[i].text);
	if((p = (byte*)(strg[STR_KEYS].text = malloc(1 + rc * 2))) == NULL) {
		fputs("Out of memory\n", stderr);
		return 89;
	}
		/* then insert the data */
	*p = 0;
	for(i = 0; i < 4; ++i) {		/* also the action code */
		memcpy(p + 1 + *p, special[i].text, j = strlen(special[i].text));
		memset(p + 1 + *p + rc, i, j);
		*p += (unsigned char)j;
	}

	/* check if the error strings are continueous */
	for(rc = 0, i = STR_ERROR; i < maxNr; ++i)
		if(!strg[i].text) {
			fprintf(stderr, "Missing error string #%d\n", i - STR_ERROR);
			rc = 48;
		}
	if(rc) return rc;
	strg[++maxNr].text = special[4].text;

	puts("CRITSTRS: Dumping CRITER strings resource");

/* Dump the stuff into a file */
	/*if((dat = fopen(fDAT, "wb")) == NULL) {*/
	if((dat = fopen(fDAT, "wt")) == NULL) {
		perror("creating " fDAT);
		return 35;
	}

	/*ska: no resource in this project! startResource(dat, RES_ID_CRITER, 2, MODULE_VERSION);*/
		/* Number of strings EXCEPT trailing "Unknown error" */
	fprintf(dat, "??strings	DB %u\n", maxNr);
	/*b = maxNr;*/
	/*fwrite(&b, sizeof(b), 1, dat);*/
		/* when loaded the pointer to the string, now offset
			within file */
	size = 1l + maxNr * 2;
	for(i = 0; i <= maxNr; ++i) {
		fprintf(dat, "\tDW S%u\n", i);
		/*w = (word)size;*/
		/*fwrite(&w, sizeof(w), 1, dat);*/
		size += i == STR_KEYS? 1 + *(byte*)strg[STR_KEYS].text * 2
			: strlen(strg[i].text) + 1;
	}
	if(size > 0xfffful) {
		fputs("Overall size of Critical Error string database exceeds 64KB\n"
		 , stderr);
		return 37;
	}
		/* Now dump the strings themselves */
	for(i = 0; i <= maxNr; ++i) {
		int l;

		fprintf(dat, "S%u\tDB ", i);
		l = i == STR_KEYS? 1 + *(byte*)strg[STR_KEYS].text * 2
		                 : strlen(strg[i].text) + 1;
		p = (unsigned char*)strg[i].text;
		if(l) {
			while(--l)
				fprintf(dat, "%u,", *p++);
			fprintf(dat, "%u\n", *p);
		}
		/*fwrite(strg[i].text, 1
		 //, i == STR_KEYS? 1 + *(byte*)strg[STR_KEYS].text * 2
		 //               : strlen(strg[i].text) + 1
		 //, dat);*/
	}

	/*ska: no resource in this project! endResource(dat);*/

	fflush(dat);
	if(ferror(dat)) {
		fputs("General write error into: " fDAT "\n", stderr);
		return 36;
	}
	fclose(dat);

	return 0;
}
