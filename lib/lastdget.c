/* $Id$

	Get Last Directory

*/

#include "../config.h"

#include <assert.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

void lastDirGet(char ** const buf)
{
	assert(buf);

	switch(ctxtGet(0, CTXT_TAG_LASTDIR, 1, buf)) {
	case 2:		error_out_of_memory();
	default:	*buf = 0;
	case 0:		break;
	}
}
