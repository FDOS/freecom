/* $Id$

	Set a specific item of the history

	Set only if the line is not empty.

*/

#include "../config.h"

#include "../include/context.h"
#include "../include/misc.h"

void histSet(const int num, const char * const str)
{
	if(!is_empty(str))
		ctxtSet(CTXT_TAG_HISTORY, num, str);
}
