/* $id$

	Make a new I context

	Return:
		internal error code
*/

#include "../config.h"

#include "../include/context.h"
#include "../include/ierror.h"

int ecMkI(void)
{
	return ecMk(EC_TAG_INTERACTIVE, 0) != 0? E_None: E_NoMem;
}
