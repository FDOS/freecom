/* $id$

	Make a new B context

	pos & lcnt default to 0

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"

int ecMkB(const char * const name)
{	unsigned idFName;
	char buf[EC_LENGTH_B];
	int rv;

	if((rv = ecMkS()) != E_None)
		return rv;

	assert(name);

	if((idFName = ecPushString(name)) == 0)
		return E_NoMem;

		/* Make "buf" the largest possible buffer size */
		/* The '|' ensures to update the two last values later on */
	sprintf(buf, "%c%u|%lu %lu|", EC_TAG_BATCH, idFName, -1L, -1L);
	assert(strlen(buf) < sizeof(buf));

		/* reset position & line number to zero */
	memcpy(strchr(buf, '|'), "|0 0|", sizeof("|0 0|") - 1);

	if((rv = ctxtPush(CTXT_TAG_EXEC, buf)) == E_None
	 && ++gflag_batchlevel == 1) {
		/* First batch level */
		gflag_echo = gflag_echoBatch;
	}

	return rv;
}
