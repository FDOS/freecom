/* $id$

	Pop and discard the topmost exec context

*/

#include "../config.h"

#include <string.h>

#include "../include/context.h"

void ecPop(void)
{	ctxtEC_t far *ec;

	if((ec = ecValidateTOS()) != 0) {
		if(ec->ctxt_type == EC_TAG_BATCH) {
			if(gflag_batchlevel > 0) {
				if(--gflag_batchlevel == 0) {
					/* Drop to interactive command line */
					gflag_echo = gflag_dispPrompt;
				}
			} else dprintf(("!! GFlag(Batchlevel) underflow\n"));
		}
#ifdef DEBUG
		if(ec->ctxt_length)
			_fmemset(ecData(ec, byte), 'K', ec->ctxt_length);
		ec->ctxt_type = 255;
#endif
		ecSetTOS((ctxtEC_t far*)((byte far*)ec
		                        + ec->ctxt_length + sizeof(*ec)));
	}
}
