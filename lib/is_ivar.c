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
{
	if(!tail) return 1;
	return ctxtGetS(0, CTXT_TAG_IVAR, tail, buf);
}
