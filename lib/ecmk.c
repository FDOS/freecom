/* $id$

	Make a new I, E, or e context

	Return:
		internal error code
*/

#include "../config.h"

#include "../include/context.h"

static char buf[2] = { 0, 0};

int ecMk(const ecTag_t tag)
{	buf[0] = tag;
	return ctxtPush(CTXT_TAG_EXEC, buf);
}
