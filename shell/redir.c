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

#include <sstr.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/misc.h"

/* The order of the strings MUST match the ones of the enum's below */
static char * redir[] = {
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

static char prefixOut[] = ">?";
static char prefixIn[] = "<";

static int appEntry(const char * const prefix
	, char *** const items
	, int * const numItems
	, char ** const line)
{	char *p, *q;

	assert(line);

	q = skippath(p = ltrimcl(*line));
	if(q == p) {
		error_empty_redirection();
		return 0;
	}
	*line = ltrimcl(q);

	return addArg(items, numItems, p, q, prefix) == E_None;
}

int get_redirection(char * const line
	, char *** const in
	, char *** const out
	, char *** const pipe)
{	int i, len;
	int numIn, numOut, num;
	char *src = line;	/* The command line is compacted by copying */
	char *dst = line;	/* it byte-wise, skipping redirections */

	assert(line);
	assert(in);
	assert(out);
	assert(pipe);

	numIn = numOut = 0;
	*in = *out = 0;
	if((*pipe = emalloc(2 * sizeof(char*))) == 0)
		return -1;
	(*pipe)[0] = src;
	num = 1;

	for(;;) {
		char *p = skipQuotedWord(src, (char*)0, "<>|");

		if(p != src) {
			if(dst != src) {
				int len;

				memmove(dst, src, len = p - src);
				dst += len;
				src = p;
			} else {
				src = dst = p;
			}
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
			*dst++ = '|';		/* stdout & stderr marker */
			/**FALL THROUGH**/
		case pipOut:
			{	char **p;

				*dst++ = 0;				/* Terminate previous command */
				if((p = erealloc(*pipe, (num + 2) * sizeof(char*))) == 0)
					goto errRet;
				*pipe = p;
				p[num++] = dst;
			}
			break;
		default:
			assert(i == noRedir);
			(*pipe)[num] = 0;
			*dst = 0;				/* Termminate last commmand */
			return num;
		}                           /* end switch */
	}

errRet:
	freep(*in);		*in = 0;
	freep(*out);	*out = 0;
	myfree(*pipe);	*pipe = 0;

	return -1;
}
