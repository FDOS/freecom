/* $id$

	Sets the contents of an internal variable

	Return:
		E_None on success

*/

#include "../config.h"

#include "../include/context.h"
#include "../include/command.h"

int ivarSet(const char * const varnam, const char * const value)
{	return ctxtSetS(CTXT_TAG_IVAR, varnam, value);
}
