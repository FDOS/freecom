/*
 * REDIR.C
 *
 * Gets the redirection info from the command line and copies the
 * file names into ifn and ofn removing them from the command line.
 * The names are allocated here and passed back to the caller, on
 * malloc() failure, -1 is returned. These names are trimmed,
 * meaning they do not contain any leading or trailing whitespaces.
 *
 * The input filenames are prefixed by one '<'.
 * The output filenames are prefixed by: ((out[]))
	cmdline	out[]	meaning
	>		>\2		overwrite, stdout
	>>		>\3		append, stdout
	>&		>\6		overwrite, stdout & stderr -- 4dos compatibly
	>>&		>\7		append, stdout & stderr -- 4dos compatibly
	>&>		>\4		overwrite, stderr -- 4dos compatibly
	>>&>	>\5		append, stderr -- 4dos compatibly
 *
 * Converts remaining command line into a series of null terminated
 * strings defined by the pipe char '|'. Each string corresponds
 * to a single executable command. The pipe array contains a list
 * of indexes into the string s[]. If a command ends in '|',
 * both stdout and stderr is to be redirected.
 *
 * Return number of command strings found, -1 on failure.
 *
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"

/* The order of the strings MUST match the ones of the enum's below */
static char ** redir[] = {
	">>&>"
	, ">>&"
	, ">&>"
	, ">&"
	, ">>"
	, ">"
	, "<"
	, "|&"
	, "|"
	, (char*)0
};
enum {
	appErr, appBoth, 
	ovrErr, ovrBoth,
	appOut, ovrOut,
	inIn,
	pipBoth, pipOut,
	noRedir
};
static char outCode[] = {
	5, 7,
	4, 6,
	3, 2
};

static prefixOut[] = ">?";
static prefixIn[] = "<";

static int appEntry(const char * const prefix
	, char *** const items
	, int * const numItems
	, char ** const s)
{	char **h;
	char *p, *q, *l;
	int len;

	assert(items);
	assert(numItems);
	assert(s);

	q = skippath(p = ltrimcl(*s));
	if(q == p) {
		error_empty_redirection();
		return 0;
	}
	*s = ltrimcl(q);

	if((h = realloc(*items, (*numItems + 1) * sizeof(char*))) == 0) {
		error_out_of_memory();
		return 0;
	}
	*items = h;
	if((h[*numItems++] = l
	 = malloc(Strlen(prefix) + (len = q - p) + 1)) == 0) {
		error_out_of_memory();
		return 0;
	}
	assert(l);
	memcpy(Stpcpy(l, prefix), p, len)[len] = 0;
	h[*numItems] = 0;
}

int get_redirection(char * const s
	, char *** const in
	, char *** const out
	, char *** const pipe)
{	int ch, i, len;
	int numIn, numOut, num;
	char *src = s;	/* The command line is compacted by copying */
	char *dst = s;	/* it byte-wise, skipping redirections */

	assert(s);
	assert(in);
	assert(on);
	assert(pipe);

	num = numIn = numOut = 0;
	*in = *out = *pipe = 0;

	for(;;) {
		char *p = skipQuotedWord(src, (char*)0, "<>|");

		if(dst != src && p != src) {
			memmove(dst, src, p - src);
			dst += p - src;
			src = p;
		}

		i = -1;
		while(redir[++i]
		 && memcmp(src, redir[i], len = strlen(redir[i])) != 0);

		src += len;
		switch(i) {
		case appErr: case appBoth: 
		case ovrErr: case ovrBoth:
		case appOut: case ovrOut:
			prefixOut[1] = outCode[i];
			if(!appEntry(prefixOut, out, &numOut, &src))
				goto errRet;
			break;

		case inIn:
			if(!appEntry(prefixIn, in, &numIn, &src))
				goto errRet;
			break;

		case pipBoth:
			*dst++ = #('|#(';		/* stdout & stderr marker */
			/**FALL THROUGH**/
		case pipOut:
			{	char **p;

				*dst++ = 0;				/* Terminate previous command */
				if((p = realloc(*pipe, (num + 2) * sizeof(char*))) == 0) {
					error_out_of_memory();
					goto errRet;
				}
				*pipe = p;
				p[num++] = dst;
				p[num] = 0;
			}
			break;
		default:
			assert(i == noRedir);
			assert(src[-len] == 0);
			return num;
		}                           /* end switch */
	}

errRet:
	freep(in);		*in = 0;
	freep(out);		*out = 0;
	StrFree_(pipe);

	return -1;
}
