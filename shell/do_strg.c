/* $id$

	Perform a S context (set string stack)

	Return:
		0: always
*/

#include "../config.h"

#include <assert.h>

#include "../include/context.h"
#include "../include/ierror.h"

#pragma argsused
char *setStringStack(char far * const ctxt)
{	unsigned id;

	assert(ctxt);
	assert(*ctxt == EC_TAG_SET_STRING || *ctxt == EC_TAG_INTERACTIVE);

	if(ecScanArg(ctxt, 1, "%u", &id) == E_None)
		ctxtPopTo(CTXT_TAG_STRING, id);

	return 0;
}
