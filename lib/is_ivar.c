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


int is_ivar(const char * const tail, char ** const buf)
{	char *name;

	if(!tail) return 1;

	if((name = regStr(emalloc(strlen(tail) + 2))) == 0)
		return 2;

	name[0] = CTXT_TAG_IVAR;
	strupr(strcpy(&name[1], tail));
	return ctxtGetItem(0, CTXT_TAG_IVAR, name, buf);
}
