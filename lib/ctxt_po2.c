/* $id$

	Pop tag items up to a specified one.
	If the specified one is zero or below nummin, the whole
	tag is cleared.

*/

#include "../config.h"

#include "../include/context.h"

void ctxtPopTo(Context_Tag tag, unsigned to_del)
{
	ctxtCheckInfoTag(tag);

	if(!to_del || to_del < CTXT_INFO(tag, nummin))
		ctxtClear(tag);
	else {
		while(to_del < CTXT_INFO(tag, nummax))
			ctxtPop(tag, (char**)0);
	}
}
