/* $id$

	Mark the special internal variable as unused and free the
	dynamically allocated buffer, too.
	This function is used to free an interal variable in the same
	run, when ecMkIVar() had been called, when something had failed
	before the internal variable had been used at all.

	Special internal variables are of the format:
		#<hexadecimal_number>

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/context.h"

void ecFreeIVar(char *ivar)
{
	ctxtGetS(1, CTXT_TAG_IVAR, ivar, (char**)0);
}
