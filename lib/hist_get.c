/* $Id$

	Set a specific item of the history

	Useage: histGet(num, buflen, string)

	num == 0 -> add a "newest" entry to the history
	num <  0 -> change the num'th newest entry of the history
	num >  0 -> change the num'th oldest entry of the history
		positive num's correspond to item ID (not relative to nummin!)
		negative ones are added to nummax
*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>

#include "../include/context.h"
#include "../include/misc.h"

int histGet(const int num, char ** const str)
{	char *p;

	obsoleted();

	assert(str);

	if(ctxtGet(0, CTXT_TAG_HISTORY, realNum(CTXT_TAG_HISTORY, num), &p) == 0) {
		assert(p);
		*str = p;
		return 1;
	}
	return 0;
}
