/* $Id$

	split a parameter line of an internal function into
	sub-arguments.

	There will be scanned at maximum argc items; argv[] must
	have at least argc entries, all non-assigned will be NULL'ed;
	empty arguments are assigned to "". argv[argc-1] will receive
	all the remaining argument (line).

	The argv[] are pointers into *line; line[] is destroyed, as
	the NUL bytes are placed there.

	Sub-arguments are delimited by NLS->listSep. Sub-arguments may be
	quoted.

*/

#include "../config.h"

#include <assert.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/ierror.h"
#include "../include/nls.h"

int splitFctLine(int argc, char ** const argv, char * const line)
{	int i;
	char *p;

	assert(argv);
	assert(argc >= 1);

	memset(argv, 0, argc * sizeof(char*));	/* zero all pointers */
	if(!line)
		return E_None;

#ifdef FEATURE_NLS
	refreshNLS();
#define delim	(char*)nlsBuf->listSep
#define skipd	strlen(delim)
#else
#define delim	","
#define skipd 1
#endif

	p = line;
	i = 0;
loop:
	p = skipQuoteStr(argv[i] = p, delim);
	if(*p && ++i < argc) {
		*p = 0;
		p += skipd;
		goto loop;
	}
	return 0;
}
