/* $id$

	Make a new S or I context

	Return:
		internal error code
*/

#include "../config.h"

#include "../include/context.h"

static mk(ecTag_t tag)
{	char buf[2 + CTXT_LENGTH_ID];

	sprintf(buf, "%c%u"
	 , tag, CTXT_INFO(CTXT_TAG_STRING, nummax));
	return ctxtPush(CTXT_TAG_EXEC, buf);
}

int ecMkS(void)
{	return mk(EC_TAG_SET_STRING);
}
int ecMkI(void)
{	return mk(EC_TAG_INTERACTIVE);
}
