/* $Id$

	Set a specific item of the history

	Useage: histSet(num, string)

	num == 0 -> add a "newest" entry to the history
	num <  0 -> change the num'th newest entry of the history
	num >  0 -> change the num'th oldest entry of the history
		positive num's correspond to item ID (not relative to nummin!)
		negative ones are added to nummax
*/

#include "../config.h"

#include "../include/context.h"
#include "../include/misc.h"

void histSet(const int num, const char * const str)
{	if(ctxtIsValid())
		ctxtSet(CTXT_TAG_HISTORY, realNum(CTXT_TAG_HISTORY, num), str);
}
