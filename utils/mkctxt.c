/*  $Id$

	Create the various file from the CONTEXT.X file
	(See there for syntax description)

	Useage: MKCTXT filename[.X]

	Produces:
	filename.h_c: contains the structure in C syntax
	filename.inc: contains the definition of the structure in assembly
	filename.def: contains the EQUs of the offsets of a value into the structure
		two definitions per offset:
		<value> - offset from the very beginning of the context
		<package>_<value> - offset of the value relative to the beginning
			of the specific package

  	$Log$
  	Revision 1.3.4.4  2001/07/25 20:17:29  skaus
  	Update #12

  	Revision 1.3.4.1  2001/07/02 21:04:07  skaus
  	Update #4
  	
  	Revision 1.3  2001/04/12 00:09:06  skaus
  	chg: New structure
  	chg: If DEBUG enabled, no available commands are displayed on startup
  	fix: PTCHSIZE also patches min extra size to force to have this amount
  	   of memory available on start
  	bugfix: CALL doesn't reset options
  	add: PTCHSIZE to patch heap size
  	add: VSPAWN, /SWAP switch, .SWP resource handling
  	bugfix: COMMAND.COM A:\
  	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
  	add: command MEMORY
  	bugfix: runExtension(): destroys command[-2]
  	add: clean.bat
  	add: localized CRITER strings
  	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
  		via STRINGS.LIB
  	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
  	add: fixstrs.c: prompts & symbolic keys
  	add: fixstrs.c: backslash escape sequences
  	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
  	chg: splitted code apart into LIB\*.c and CMD\*.c
  	bugfix: IF is now using error system & STRINGS to report errors
  	add: CALL: /N
  	
  	Revision 1.1.2.2  2001/02/18 21:08:24  skaus
  	add: command WHICH
  	fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
  	
  	Revision 1.1.2.1  2001/02/18 17:59:37  skaus
  	bugfix: KSSF: restore parentPSP on exit
  	chg: Using STRINGS resource for all non-interactive messages
  	chg: moving all assembly files to NASM
  	
  	Revision 1.1.2.1  2000/12/17 21:57:37  skaus
  	intermediate update 1
  	
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static char *typename = NULL;	/* Current typedef */
static char *pkg = NULL;	/* Current package */
static char *stru = NULL;	/* Current structure */
static unsigned pkgOffs = 0, offset = 0;	/* package and overall offset */
static char *fnam = NULL;	/* filename with spare room for extension */
static char *fext;			/* pointer to extension of fnam */
static char buf[1024];
static unsigned lineNr = 0;
static FILE *fincc, *finca, *fdefa, *fx;

static char**types = 0;
static int typenum = 0;
static char**inits = 0;
static int initnum = 0;
static int init_firstval = 0, init_record = 0;

static char *mkStruName(const char * const name)
{	static char*last = 0;

	free(last);
	last = malloc(strlen(name) + 3);
	if(!last) {
		puts("Out of memory");
		exit(120);
	}
	strcpy(stpcpy(last, name), "_t");
	*last = toupper(*last);

	return last;
}

static char *skipws(char *p)
{	while(*p && isspace(*p))
		++p;
	return p;
}
static void incOffs(int size)
{	offset += size;
	pkgOffs += size;
}
static void dumpDefine(char *name, char *value)
{	long i;

	i = strtol(value, 0, 0);
	fprintf(fincc, "#define %s %ld\n", name, i);
	fprintf(finca, "%%define %s %ld\n", name, i);
}
static void dumpEQU(char *symbol)
{	fprintf(finca, "?%s EQU %u\n", symbol, offset);
	fprintf(fdefa, "??%s:\n", symbol);

	if(pkg && pkg != symbol)
		fprintf(finca, "?%s_%s EQU %u\n", pkg, symbol, pkgOffs);
}
static void indent(void)
{	if(typename) putc('\t', fincc);
	if(pkg)	putc('\t', fincc);
	if(stru)	putc('\t', fincc);
}
static void endStru(void)
{	if(stru) {
		char *p;
		p = stru;
		stru = NULL;
		indent();
		fprintf(fincc, "} %s;\n", p);
		free(p);
		if(init_record) {
			++initnum;
			init_firstval = init_record = 0;
		}
	}
}
static void endPkg(void)
{	if(pkg) {
		fprintf(fincc, "\t} %s;\n", pkg);
		free(pkg);
		pkg = NULL;
	}
}
static void endType(void)
{
	if(typename) {
		fprintf(fincc, "} %s;\n\n", typename);
	}

	if(types && typenum) {
		int i;

		for(i = 0; i < typenum; ++i) {
			fprintf(fincc, "typedef struct %s %s;\n"
			 , mkStruName(types[i]), types[i]);
			free(types[i]);
		}
		free(types);
		types = 0;
		typenum = 0;
	}

	if(inits && initnum) {
		int i;

		for(i = 0; i < initnum; ++i) {
			fputs("#define init_", fincc);
			fputs(inits[i], fincc);
			fputs("}\n", fincc);
			free(inits[i]);
		}
		free(inits);
		inits = 0;
		initnum = 0;
	}

	if(typename) {
		fputs("#include <algndflt.h>\n", fincc);
		free(typename);
		typename = NULL;
	}
}

static void dumpComment(char *comment)
{	unsigned char *p;

	if(!comment || !*(comment = skipws(comment)))
		return;
	p = (unsigned char*)strchr(comment, '\0');
	while(--p >= (unsigned char*)comment && *p <= ' ');
	p[1] = 0;
	if(p == comment - 1)
		return;
	fprintf(fincc, "/* %s */\n", comment);
	fprintf(fdefa, ";; %s\b", comment);
	fprintf(finca, ";; %s\b", comment);
}
static void dumpItem(char *name, char *value, char *Ctype, char asmType)
{	dumpEQU(name);
	indent();
	fprintf(fincc, "%s %s;\n", Ctype, name);
	fprintf(fdefa, "\tD%c %s\n", asmType, value);
	if(init_record) {
		if((inits[initnum] = realloc(inits[initnum]
		 , strlen(inits[initnum]) + strlen(value) + 5)) == 0) {
		 	puts("Out of memory");
		 	exit(121);
		}
		if(!init_firstval)
			strcat(inits[initnum], ",\\\n\t");
		else
			init_firstval = 0;
		strcat(inits[initnum], value);
	}
}
#define word word_(&p)
static char *word_(char **Xp)
{	char *p, *q;

	q = p = skipws(*Xp);
	if(!*p || *p == '#') {
		printf("Syntax error in line %u: Missing argument\n", lineNr);
		exit(62);
	}
	while(*++q && !isspace(*q));
	if(*q) {
		*q = '\0';
		*Xp = skipws(q + 1);
	} else
		*Xp = q;
	return p;
}
#define match(w) match_(&p, (w))
static int match_(char **Xp, char *w)
{	char *p, *q;

	q = p = skipws(*Xp);
	if(!*p || *p == '#') {
		printf("Syntax error in line %u: Missing keyword\n", lineNr);
		exit(62);
	}
	while(*++q && !isspace(*q));
	if(strlen(w) == q - p && memicmp(w, p, q - p) == 0) {
		/* equal */
		if(*q)	*Xp = skipws(q + 1);
		else	*Xp = q;
		return 1;
	}
	return 0;
}

main(int argc, char **argv)
{
	char *p, *name, *value;
	int err;

	if(argc != 2) {
		puts("MKCTXT - Create definition file from *.X files\n\n"
			"Useage: MKCTXT filename[.X]\n");
		return 127;
	}

	if((fnam = strdup(argv[1])) == NULL
	 || (fnam = realloc(fnam, strlen(fnam) + 4)) == NULL) {
	 	puts("Out of memory");
	 	return 120;
	}

	fext = strchr(fnam, '\0');
	while(--fext >= fnam && *fext != '.' && *fext != '/' && *fext != '\\');
	if(fext < fnam || *fext != '.') {
		fext = strchr(fnam, '\0');
		strcpy(fext++, ".x");
	} else ++fext;

	if((fx = fopen(fnam, "rt")) == NULL) {
		printf("Cannot open input file: %s\n", fnam);
		return 40;
	}
	strcpy(fext, "h_c");
	if((fincc = fopen(fnam, "wt")) == NULL) {
		printf("Cannot create C include file: %s\n", fnam);
		return 41;
	}
	strcpy(fext, "inc");
	if((finca = fopen(fnam, "wt")) == NULL) {
		printf("Cannot create assembly include file: %s\n", fnam);
		return 42;
	}
	strcpy(fext, "def");
	if((fdefa = fopen(fnam, "wt")) == NULL) {
		printf("Cannot create assembly definition file: %s\n", fnam);
		return 43;
	}

	/* Dump the headers into the files */
	/* C include file: autogenerated notice and one-byte alignment */
	fputs("/* This is an autogenerated file.\n"
		"\tAny modifications are lost once MKCTXT runs the next time.\n"
		"\tModify the particular *.X source file instead. */\n\n"
		"#ifndef _CONTEXT_H_C_\n"
		"#define _CONTEXT_H_C_\n", fincc);
	fputs(";; This is an autogenerated file.\n"
		";;\tAny modifications are lost once MKCTXT runs the next time.\n"
		";;\tModify the particular *.X source file instead. \n\n", finca);
	fputs(";; This is an autogenerated file.\n"
		";;\tAny modifications are lost once MKCTXT runs the next time.\n"
		";;\tModify the particular *.X source file instead. \n\n", fdefa);

	while(fgets(buf, sizeof(buf), fx)) {
		++lineNr;
		p = skipws(buf);
		if(!*p) {
			endStru();
			continue;
		}
		if(*p == '#') {
			if(memcmp(p, "###>", 4) == 0)
				dumpComment(p + 4);
			continue;
		}

		if(match("typedef")) {
			endStru();
			endPkg();
			endType();
			typename = strdup(word);
			if(!typename) {
				puts("Out of memory");
				return 120;
			}
		
			fprintf(fincc, "\n#include <algnbyte.h>\n\n"
			 "typedef struct {\t/* type %s */\n", typename);
		} else if(match("package")) {
			pkgOffs = 0;
			endStru();
			endPkg();
			indent();
			pkg = strdup(word);
			if(!pkg) {
				puts("Out of memory");
				return 120;
			}
			fprintf(fincc, "struct {\t/* package %s */\n", pkg);
			dumpEQU(pkg);
		} else if(match("structure")) {
			char *typename;

			endStru();
			indent();
			stru = strdup(word);
			if(!stru) {
				puts("Out of memory");
				return 120;
			}
			if(*p && *p != '#') {
				typename = strdup(word);
				if(!typename) {
					puts("Out of memory");
					return 120;
				}
				if((types = realloc(types, (typenum + 1) * sizeof(char*)))
				 == 0) {
					puts("Out of memory");
					return 120;
				}
				types[typenum++] = typename;
				fprintf(fincc, "struct %s {\t/* structure %s */\n"
				 , mkStruName(typename), stru);
				if((inits = realloc(inits, (initnum + 1) * sizeof(char*)))
				 == 0
				 || (inits[initnum] = malloc(strlen(stru) + 3)) == 0) {
					puts("Out of memory");
					return 120;
				}
				strcpy(stpcpy(inits[initnum], stru), " {");
				init_record = init_firstval = 1;
			} else {
				fprintf(fincc, "struct {\t/* structure %s */\n", stru);
			}
			dumpEQU(stru);
		} else if(match("byte")) {
			name = word;
			value = word;
			dumpItem(name, value, "byte", 'B');
			incOffs(1);
		} else if(match("word")) {
			name = word;
			value = word;
			dumpItem(name, value, "word", 'W');
			incOffs(2);
		} else if(match("pointer")) {
			name = word;
			value = word;
			dumpItem(name, value, "void far*", 'D');
			incOffs(4);
		} else if(match("reference")) {
			name = word;
			dumpEQU(name);
			indent();
			fprintf(fincc, "unsigned char %s[];\n", name);
		} else if(match("constant")) {
			name = word;
			value = word;
			dumpDefine(name, value);
		} else {
			printf("Syntax error in line %u: Invalid keyword\n", lineNr);
			return 60;
		}

		if(*p && *p != '#') {
			printf("Syntax error in line %u: Too many arguments\n", lineNr);
			return 61;
		}
	}

	endStru();
	endPkg();
	endType();

	fputs("\n#endif\n", fincc);

	fflush(fincc);
	fflush(finca);
	fflush(fdefa);

	err = 0;
	if(ferror(fx)) {
		strcpy(fext, "x");
		printf("Read error from: %s\n", fnam);
		err = 50;
	}
	if(ferror(fincc)) {
		strcpy(fext, "h_c");
		printf("Write error to: %s\n", fnam);
		err = 51;
	}
	if(ferror(finca)) {
		strcpy(fext, "inc");
		printf("Write error to: %s\n", fnam);
		err = 52;
	}
	if(ferror(fdefa)) {
		strcpy(fext, "def");
		printf("Write error to: %s\n", fnam);
		err = 53;
	}
	
	fclose(fx);
	fclose(fincc);
	fclose(finca);
	fclose(fdefa);

	return err;
}
