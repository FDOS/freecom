/* $id$

	Make a new f context

	varname and cmd are inserted as offsets relative to ec's segment.

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>
#include <dynstr.h>
#include <suppl.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int ecMkf(const void * const ffblk
	, const unsigned idVarname
	, const unsigned idCmd
	, const char * const prefix)
{	unsigned idFfblk, idPrefix;
	char *p;
	char buf[CTXT_LENGTH_ID * 4 + 2];

	assert(ffblk);
	assert(idVarname);
	assert(idCmd);

	if(prefix) {
		if((idPrefix = ecPushString(prefix)) == 0)
			return E_NoMem;
	} else
		idPrefix = 0;

			/* 21: length of ffblk structure required for findnext() */
	if((p = esEncMem(ffblk, 21)) == 0) {
		error_out_of_memory();
		return E_NoMem;
	}
	idFfblk = ecPushString(p);
	myfree(p);
	if(!idFfblk)
		return E_NoMem;

	sprintf(buf, "%c%u %u %u %u"
	 , EC_TAG_FOR_NEXT, idFfblk, idVarname, idCmd, idPrefix);
	assert(strlen(buf) < sizeof(buf));

	return ctxtPush(CTXT_TAG_EXEC, buf);
}
