/* $id$

	Make a new I or S context

	Return:
		internal error code
*/

#include "../config.h"

#include "../include/context.h"
#include "../include/ierror.h"

int ecMkI_S(const byte tag)
{	char buf[2 + CTXT_LENGTH_ID];

	sprintf(buf, "%c%u" , tag, CTXT_INFO(CTXT_TAG_STRING, nummax));
	return ctxtPush(CTXT_TAG_EXEC, buf);
}
