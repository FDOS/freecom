/*
This program will read in the language file and create two files:

strings.dat - contains all the strings. will be appended on the end
              of the exe file.

strings.h - include file that contains all the defines.

There are two input files:
DEFAULT.LNG and the language file passed as argument to FIXSTRS.
DEFAULT.LNG has two meanings, which make it fundamental file, which
ensures the integrity of the multi-language support of FreeCOM:

1) The order of strings noted in DEFAULT.LNG will be kept the same in
	all *.LNG files.
2) If an individual *.LNG file does not define a certain string, its
	contents is taken from the DEFAULT file.

Because each string is assigned a certain number and only this number
is known to FreeCOM internally, especially meaning 1) will ensure that
each STRINGS.DAT assigns the same semantic to those numbers.

The STRINGS.DAT file generated in the following steps:
1) DEFAULT.LNG is read; all strings are copied into memory,
	the string_index_t array is generated with these strings.
	At the end, one could generate STRINGS.DAT with all the default
	strings, which are usually in English.
2) the specified *.LNG file is read; if a string defined in this file,
	too, its contents overwrites the default string already present.
	New strings are assigned a new number above the already allocated
	string numbers.
3) Now all strings are known, there sizes and offsets within the file,
	the STRINGS.DAT and STRINGS.H files are generated.

2000/07/09 ska
chg: to read STRINGS.H to keep the same order of strings
chg: to let STRINGS.TXT read only once (temporary binary file)
chg: The format of STRINGS.DAT has been changed in order to support
	different languages _without_ recompiling.

2000/07/11 ska
chg: To use STRINGS.H to keep up the order becomes problematic, as this
	file is regenerated each time FIXSTRS is run. On failure, this
	file is destroyed. Therefore STRINGS.TXT will be renamed into
	DEFAULT.LNG and is used to a) specify the order and, if missing,
	the default string text.
*/

#define MODULE_VERSION 0

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../strings.typ"
#include "../resource.h"

#define fDAT "STRINGS.DAT"
#define fTXT "DEFAULT.LNG"
#define fH "STRINGS.H"
#define fEXT ".LNG"

typedef enum STATE {
	 LOOKING_FOR_START
	,GETTING_STRING
} read_state;

#define MAXSTRINGS       256

const char id[]="FreeDOS STRINGS v";


/*
	Implementation details about to cache the strings within memory:

	+ Because the strings are currently entitled to fit into 64KB,
		they should fit into memory during runtime of this program.
		Therefore FIXSTRS will be compiled in Compact memory model.

*/

string_index_t string[MAXSTRINGS];
struct {
	char *name;				/* name of string */
	char *text;				/* text of this string */
} strg[MAXSTRINGS];
string_count_t cnt = 0;		/* current string number */
string_count_t maxCnt = 0;	/* number of strings within array */

#if sizeof(char*) != 4
#error "This program must be compiled with far data pointers"
#endif

char temp[256];

	/* keep it a single-file project */
#include "../res_w.c"

/*
 * Append the passed in string onto strg[cnt].text
 */
int app(char *s)
{
	if(!strg[cnt].text) {
		if((strg[cnt].text = strdup(s)) != 0) {
			string[cnt].size = strlen(strg[cnt].text) + 1;
			return 1;
		}
	} else if((strg[cnt].text = realloc(strg[cnt].text
	 , string[cnt].size += strlen(s))) != NULL) {
		strcat(strg[cnt].text, s);
		return 1;
	}

	fputs("Out of memory\n", stderr);
	return 0;
}

#define join(s1,s2)	strcpy(stpcpy(temp, s1), s2);
void pxerror(char *msg1, char *msg2)
{	join(msg1, msg2);
	perror(temp);
}

int loadFile(char *fnam)
{	unsigned long linenr;
	char *p;
	read_state state = LOOKING_FOR_START;
	FILE *fin;

	join(fnam, fEXT);
	if((fin = fopen(fnam, "rt")) == NULL
	 && (fin = fopen(temp, "rt")) == NULL) {
		pxerror("opening ", fnam);
		return 33;
	}

	linenr = 0;
	while (fgets(temp, sizeof(temp), fin)) {
		++linenr;
		p = strchr(temp, '\0');
		if(p[-1] != '\n') {
			fprintf(stderr, "Line %lu too long\n", linenr);
			return 41;
		}
			/* Cut trailing control characters */
		while (--p >= temp && *p < ' ');
		p[1] = '\0';

		switch (state) {
		case LOOKING_FOR_START:
			switch(*temp) {
			case ':':
				state = GETTING_STRING;
				/* Locate the string name */
				for(cnt = 0; cnt < maxCnt; ++cnt)
					if(strcmp(strg[cnt].name, temp + 1) == 0)
						goto strnameFound;
				/* string name was not found --> create a new one */
				++maxCnt;
			strnameFound:
				free(strg[cnt].name);		/* purge assignments */
				free(strg[cnt].text);		/* of previous run */
				if((strg[cnt].name = strdup(temp + 1)) == NULL) {
					fputs("Out of memory\n", stderr);
					return 80;
				}
				strg[cnt].text = NULL;
			break;
			default:
				while(p >= temp && isspace(*p)) --p;
				if(p >= temp) {
					fprintf(stderr, "Syntax error in line #%lu\n", linenr);
					return 44;
				}
				/** fall through **/
			case '\0': case '#':
				break;
			}
		break;

		case GETTING_STRING:
			if ((*temp == '.' || *temp == ',') && (temp[1] == '\0')) {
				if (*temp == '.' &&	!app("\n"))
					return 81;
				state = LOOKING_FOR_START;
			} else {
				if(strg[cnt].text && !app("\n"))
					return 81;
				if(!app(temp))
					return 82;
			}
		break;
		}
	}
	if(ferror(fin)) {
		pxerror("reading ", fnam);
		return 34;
	}
	fclose(fin);
	if(state == GETTING_STRING) {
		fprintf(stderr, "%s: Last string not terminated\n", fnam);
		return 40;
	}

	return 0;
}


int main(int argc, char **argv)
{
	FILE *dat, *inc;
	int rc;
	unsigned long size;
	string_count_t cnt;		/* current string number */
	string_size_t lsize;


	if(argc > 2) {
		puts("FIXSTRS - Generate STRINGS.DAT and STRINGS.H for a language\n"
			"Useage: FIXSTRS [language]\n"
			"\tIf no language is specified, the default strings are read only.\n"
			"\tThere must be <language>.LNG file in the current directory.\n"
			"Note: DEFAULT.LNG must be present in the current directory, too.");
		return 127;
	}



	if((rc = loadFile(fTXT)) != 0)
		return rc;
	if(argc > 1 && (rc = loadFile(argv[1])) != 0)
		return rc;

/* Now all the strings are cached into memory */

	if(maxCnt < 2) {
		fputs("No string definition found.\n", stderr);
		return 43;
	}

	/* 1. Adjust the offset and generate the overall size */
	for(size = string[0].size, cnt = 1; cnt < maxCnt; ++cnt) {
		string[cnt].index = string[cnt-1].index + string[cnt-1].size;
		size += string[cnt-1].size;
	}

	if(size >= 0x10000ul - sizeof(string_index_t) * maxCnt) {
		fputs("Overall size of strings exceeds 64KB limit\n", stderr);
		return 44;
	}

	/* 2. Open STRINGS.DAT and STRINGS.H and dump control information */
	if ((dat = fopen(fDAT,"wb")) == NULL) {
		perror("creating " fDAT);
		return 36;
	}
	if ((inc = fopen(fH,"wt")) == NULL) {
		perror("creating " fH);
		return 37;
	}

	fputs("/*\n"
		" * This file was automatically generated by FIXSTRS.\n"
		" * Any modifications will be lost next time this tool\n"
		" * is invoked.\n"
		" */\n\n", inc);
	fprintf(inc,"#define  STRINGS_ID         \"%s%u\"\n"
	 , id, STRING_RESOURCE_MINOR_ID);
#if 0		/* Note: Superceeded by embedded parameters */
	fprintf(inc,"#define  NUMBER_OF_STRINGS  0x%02X\n",maxCnt);
	fprintf(inc,"#define  SIZE_OF_STRINGS    0x%04X\n", (unsigned)size);
#endif

	startResource(dat, RES_ID_STRINGS, STRING_RESOURCE_MINOR_ID
		, MODULE_VERSION);
		/* Preamble of STRINGS.DAT file */
	fprintf(dat, "%s%u", id, STRING_RESOURCE_MINOR_ID);
/*	fwrite(id, sizeof(id) - 1, 1, dat);		*//* file contents ID */
	fwrite("\r\n\x1a", 4, 1, dat);			/* text file full stop */
	fputs("#define  STRINGS_ID_TRAILER 4\n", inc);	/* 4 additional bytes */
	fputs("\n\n", inc);						/* delimiter */

		/* parameters of strings */
	fwrite(&maxCnt, sizeof(maxCnt), 1, dat);	/* number of strings */
	lsize = (string_size_t)size;
	fwrite(&lsize, sizeof(lsize), 1, dat);		/* total size of string text */

		/* string control area */
	fwrite(string, sizeof(string[0]), maxCnt, dat);
	/* append the strings */
	for(cnt = 0; cnt < maxCnt; ++cnt) {
		fwrite(strg[cnt].text, string[cnt].size, 1, dat);
		fprintf(inc, "#define  %-34s 0x%02x  /* @ 0x%04x */\n"
		 , strg[cnt].name, cnt, string[cnt].index);
	}
	fputs("\n/* END OF FILE */\n", inc);
	endResource(dat);

	fflush(dat);
	if(ferror(dat)) {
		fputs("Unspecified write error into " fDAT "\n", stderr);
		return 38;
	}
	fflush(inc);
	if(ferror(inc)) {
		fputs("Unspecified write error into " fH "\n", stderr);
		return 39;
	}

	fclose(dat);
	fclose(inc);

	return 0;
}
