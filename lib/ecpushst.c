/* $id$

	Create a new "String" entry and display any error message.

	return:
		0 on failure
		else: string ID
*/

#include "../config.h"

#include "../include/context.h"
#include "../include/ierror.h"

unsigned ecPushString(const char * const str)
{	if(ctxtPush(CTXT_TAG_STRING, str) == E_None)
		return CTXT_INFO(CTXT_TAG_STRING, nummax);

	return 0;
}
