/* $Id$

	Expand aliases

*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <environ.h>
#include <fmemory.h>
#include <nls_f.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

void aliasexpand(char * const cmd, const int maxlen)
{	char *hbuf;				/* work buffer */
	char *cp, *p;
	unsigned ofs, *expanded, *he;
	int i, numExpanded;

	assert(cmd);
	assert(strlen(cmd) < maxlen);

	if((hbuf = malloc(maxlen)) == 0) {
		error_out_of_memory();
		goto errRet;
	}
	numExpanded = 0;
	expanded = 0;

redo:						/* iteration to expand all aliases */
	cp = ltrim(cmd);		/* skip leading whitespaces */

	/* Check if the user disabled alias expansion */
	if(*cp == '*') {
		cp = ltrim(cp + 1);
		memmove(cmd, cp, strlen(cp) + 1);
		goto errRet;
	}

	/* Get the name of this command */
	for(p = hbuf; is_fnchar(*cp) || *cp == '.';)
		*p++ = *cp++;
	if(p == hbuf || is_pathdelim(*cp) || is_quote(*cp))
		/* no name || part of path -> no alias */
		goto errRet;

	*p = 0;
	StrFUpr(hbuf);			/* all aliases are uppercased */
	if((ofs = env_findVar(ctxtAlias, hbuf)) == (unsigned)-1)
		/* not found -> no alias */
		goto errRet;

	/* Prevent recursion by recording the offset of the found variable */
	for(i = 0; i < numExpanded; ++i)
		if(expanded[i] == ofs)		/* already used -> ignore */
			goto errRet;

	if((he = realloc(expanded, ++numExpanded)) == 0) {
		error_out_of_memory();
		goto errRet;
	}
	expanded = he;
	expanded[numExpanded - 1] = ofs;

	/************ Expand the command line "cp" with the alias at
						MK_FP(ctxtAlias, ofs)  ***********************/
	ofs += strlen(hbuf) + 1;		/* advance to value */
	if(_fstrlen(MK_FP(ctxtAlias, ofs)) < maxlen - strlen(cp)) {
		/* prepend alias value to remaining command line */
		_fstrcpy(TO_FP(hbuf), MK_FP(ctxtAlias, ofs));
		strcat(hbuf, cp);
		strcpy(cmd, hbuf);
		goto redo;				/* next expansion */
	}

	error_command_too_long();

errRet:							/* return to caller */
	free(expanded);
	free(hbuf);
}
