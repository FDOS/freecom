/* $id$

	Pop and discard the topmost exec context

*/

#include "../config.h"

#include <string.h>

#include "../include/context.h"

void ecPop(void)
{	char *buf;

	if(ctxtPop(CTXT_TAG_EXEC, &buf)) {
		assert(buf);
		switch(*buf) {
		case EC_TAG_BATCH:
			if(gflag_batchlevel-- == 0) {
				dprintf(("[CTXT: Batch nestinglevel underflow]\n"));
				gflag_batchlevel = 0;
			}
			break;
		}
		myfree(buf);
	}
}
