/* $Id$

	Test and return an internal variable

	Return:
		0: OK
		1: no such variable
		2: out of memory (error already displayed)
*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/context.h"
#include "../err_fcts.h"
#include "../include/misc.h"


int is_ivar(const char * const name, char ** const buf)
{	char *tagname;
	int rv;

	assert(buf);

	if(!name) return 1;

	if((tagname = emalloc(strlen(name) + 2)) != 0) {
		*tagname = CTXT_TAG_IVAR;
		strcpy(&tagname[1], name);
		strupr(&tagname[1]);
		rv = ctxtGetS(0, CTXT_TAG_IVAR, tagname, buf);
		myfree(tagname);
		if(rv < 2)
			return rv;
	}

	return 2;
}
