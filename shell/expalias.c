/* $Id$

	Expand aliases

*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>
#include <environ.h>
#include <fmemory.h>

#include "../include/cmdline.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../include/nls.h"
#include "../err_fcts.h"

char *aliasexpand(const char * const Xcmd)
{	char *cmd;				/* work buffer */
	char *cp, *name;
	unsigned ofs, *expanded, *he;
	int i, numExpanded, newlen, len;

	assert(Xcmd);

	if((cmd = strdup(Xcmd)) == 0) {
		error_out_of_memory();
		return (char*)Xcmd;
	}

	numExpanded = 0;
	expanded = 0;
	name = 0;

redo:						/* iteration to expand all aliases */
	cp = ltrimcl(cmd);		/* skip leading whitespaces */

	/* Check if the user disabled alias expansion */
	if(*cp == '*') {
		++cp;
		memmove(cmd, cp, strlen(cp) + 1);
		goto errRet;
	}

	myfree(name);
	/* Get the name of this command */
	if((name = getCmdName(&(const char*)cp)) == 0 || is_pathdelim(*cp))
		goto errRet;

	StrFUpr(name);			/* all aliases are uppercased */
	if((ofs = env_findVar(ctxtAlias, name)) == (unsigned)-1)
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
	ofs += strlen(name) + 1;		/* advance to value */
		/* Check that the total command line won't overflow MAX_INT */
	if((newlen = _fstrlen(MK_FP(ctxtAlias, ofs))) >= 0
	 && (len = strlen(cp)) >= 0 && ++len > 0 && newlen + len > 0) {
	 	int dst = cp - cmd;		/* destination index within cmd[] */
	 	if(newlen > dst) {
	 		/* need to increase the buffer */
	 		char *p;

	 		if((p = realloc(cmd, newlen + len)) == 0)
	 			goto errRet1;

	 		cmd = p;
	 	}
	 	/* else ignore to shrink the buffer; it will be done
	 		automatically with the next ALIAS expansion or when
	 		this function returns */
	 	assert(len);
	 	if(newlen != dst)
	 		/* need to move the command tail */
	 		memmove(&cmd[newlen], &cmd[dst], len);

		/* prepend alias value to remaining command line */
		_fmemcpy(TO_FP(cmd), MK_FP(ctxtAlias, ofs), newlen);
		goto redo;				/* next expansion */
	}

errRet1:
	error_long_internal_line();

errRet:							/* return to caller */
	myfree(expanded);
	myfree(name);

	return StrTrim(cmd);
}
