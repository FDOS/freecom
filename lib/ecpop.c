/* $id$

	Pop and discard the topmost exec context

*/

#include "../config.h"

#include "../include/context.h"

void ecPop(void)
{	ctxtEC_t far *ec;

	if((ec = ecValidateTOS()) != 0) {
		if(ec->ctxt_type == EC_TAG_BATCH) {
			if(F(batchlevel) > 0)
				--F(batchlevel);
			else dprintf(("!! Flag(Batchlevel) underflow\n"));
		}
		ecSetTOS((ctxtEC_t far*)((byte far*)ec
		                        + ec->ctxt_length + sizeof(*ec)));
	}
}
