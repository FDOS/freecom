/* $id$

	Set the top of string stack

*/

#include "../config.h"

#include <assert.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

void ecSetStringStack(const char far * const ctxt)
{	unsigned id;

	if(ecScanArg(ctxt, 1, "%u", &id) == E_None)
		ctxtPopTo(CTXT_TAG_STRING, id);
}
