/*
This program will read in the strings.txt file and create two files:

strings.dat - contains all the strings. will be appended on the end
              of the exe file.

strings.h - include file that contains all the defines.


2000/07/09 ska
chg: to read STRINGS.H to keep the same order of strings
chg: to let STRINGS.TXT read only once (temporary binary file)
chg: The format of STRINGS.DAT has been changed in order to support
	different languages _without_ recompiling.
*/


#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../strings.typ"

#define fTMP "STRINGS.TMP"
#define fDAT "STRINGS.DAT"
#define fTXT "STRINGS.TXT"
#define fH "STRINGS.H"
#define fBAK "STRINGS.BAK"

typedef enum STATE {
	 LOOKING_FOR_START
	,GETTING_STRING
} read_state;

#define MAXSTRINGS       256

const char id[]="FreeDOS STRINGS.DAT v2";

string_index_t string[MAXSTRINGS];
char *order[MAXSTRINGS];
char temp[256];

int main(void)
{
  FILE *in, *dat, *inc, *tmp;

  read_state state = LOOKING_FOR_START;
  int firstline;
  char *p;
  unsigned long linenr;
  string_size_t size;
  string_count_t cnt, maxCnt;
  string_count_t oldCnt = 0;
  string_count_t prvCnt = STRING_NONE;
  	/* oldCnt := number of strings read from STRINGS.H
  		cnt := number of current string
  		prvCnt := number of previous string to correct .size member;
  				== (unsigned)-1 if first string
  		maxCnt := total number of strings, including new ones
  	*/



  if ((in = fopen(fTXT,"r")) == NULL)
  {
    perror("opening " fTXT);
    return 40;
  }

  if ((dat = fopen(fDAT,"wb")) == NULL)
  {
    perror("creating " fDAT);
    return 41;
  }
  if((tmp = fopen(fTMP, "w+b")) == NULL) {
  	perror("creating " fTMP);
  	return 43;
  }


	if((inc = fopen(fH, "rt")) == NULL)
		fputs("Warning: No STRINGS.H file <-> Possible strings order hazard\n"
		 , stderr);
	else {
		char *q;

		while (fgets(temp,sizeof(temp),inc)) {
			p = strchr(temp, '\0');
			if(p[-1] != '\n') {
				fputs("Line too long in " fH "\n", stderr);
				return 70;
			}
			if(memcmp(temp, "#define ", 8) != 0)
				break;
			p = temp + 7;
			while(isspace(*++p));
			q = p;
			while(*++q && !isspace(*q));
			if(!*q) {
				fputs("Syntax error in " fH "\n", stderr);
				return 71;
			}
			*q = '\0';
			if((order[oldCnt++] = strdup(p)) == NULL) {
				fputs("Out of memory\n", stderr);
				return 72;
			}
		}
		fclose(inc);
		remove(fBAK);
		if(rename(fH, fBAK)) {
			fputs("Failed to backup " fH "\n", stderr);
			return 75;
		}
	}
	maxCnt = oldCnt;
  if ((inc = fopen(fH,"w")) == NULL)
  {
    perror("creating " fH);
    return 42;
  }

	linenr = 0;
	while (fgets(temp,sizeof(temp),in)) {
		++linenr;
		p = strchr(temp, '\0');
		if(p[-1] != '\n') {
			fprintf(stderr, "Line %lu too long\n", linenr);
			return 30;
		}
			/* Cut trailing control characters */
		while (--p >= temp && *p < ' ');
		p[1] = '\0';

		switch (state) {
		case LOOKING_FOR_START:
			switch(*temp) {
			case ':':
				state = GETTING_STRING;
				if(ftell(tmp) >= 0x10000l) {
					fputs("The overall size of the strings exceeds 64KB\n"
					 , stderr);
					return 35;
				}
				/* Locate the string name */
				for(cnt = 0; cnt < oldCnt; ++cnt)
					if(strcmp(order[cnt], temp + 1) == 0)
						goto strnameFound;
				/* string name not found --> create a new one */
				cnt = maxCnt++;
			strnameFound:
				string[cnt].index = (string_count_t)ftell(tmp);
				if (prvCnt != STRING_NONE)
					string[prvCnt].size = string[cnt].index
					 - string[prvCnt].index;
				fprintf(inc, "#define  %-34s 0x%02X /* @ 0x%04X */\n"
				 , temp+1, cnt, string[cnt].index);
				prvCnt = cnt;
				firstline = 1;
			break;
			default:
				while(p >= temp && isspace(*p)) --p;
				if(p >= temp) {
					fprintf(stderr, "Syntax error in line #%lu\n", linenr);
					return 36;
				}
				/** fall through **/
			case '\0': case '#':
				break;
			}
		break;

		case GETTING_STRING:
			if ((*temp == '.' || *temp == ',') && (temp[1] == '\0')) {
				if (*temp == '.') fputc('\n', tmp);
				fputc('\0', tmp);
				state = LOOKING_FOR_START;
			} else {
				if (!firstline) fputc('\n', tmp);
				firstline = 0;
				fputs(temp, tmp);
			}
		break;
		}
	}

	if(prvCnt == STRING_NONE) {
		fputs("No string defined\n", stderr);
		return 37;
	}
	string[prvCnt].size = (string_size_t)ftell(tmp) - string[prvCnt].index;

	if(ftell(tmp) >= 0x10000l) {
		fputs("The overall size of the strings exceeds 64KB\n"
		 , stderr);
		return 35;
	}

	fflush(tmp);
	if(ferror(tmp)) {
		fputs("Unspecified write error to " fTMP "\n", stderr);
		return 50;
	}

	fputs("\n\n", inc);
	fprintf(inc,"#define  STRINGS_ID         \"%s\"\n",id);
	fprintf(inc,"#define  NUMBER_OF_STRINGS  0x%02X\n",maxCnt);
	fprintf(inc,"#define  SIZE_OF_STRINGS    0x%04X\n"
	 , size = (unsigned)ftell(tmp));

	rewind(tmp);

	fwrite(id, sizeof(id) - 1, 1, dat);
	fwrite("\n\x1a", 3, 1, dat);
	fputs("#define  STRINGS_ID_TRAILER 3\n", inc);	/* 3 additional bytes */

	fwrite(&maxCnt, sizeof(maxCnt), 1, dat);
	fwrite(&size, sizeof(size), 1, dat);
	fwrite(string, sizeof(string[0]), maxCnt, dat);
	/* append the strings */
	while((cnt = fread(temp, 1, sizeof(temp), tmp)) >= 1) {
		if(fwrite(temp, 1, cnt, dat) != cnt) {
			fputs("Write error into " fDAT "\n", stderr);
			return 53;
		}
	}
	fflush(dat);
	if(ferror(dat)) {
		fputs("Unspecified write error into " fDAT "\n", stderr);
		return 54;
	}

	fclose(in);
	fclose(dat);
	fclose(inc);
	fclose(tmp);
	unlink(fTMP);

	return 0;
}
