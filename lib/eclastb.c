/* $id$

	Return a pointer to the topmost B context

	Return:
		0: on failure, message already displayed
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>

#include "../include/context.h"
#include "../include/misc.h"

ctxtEC_Batch_t far*ecLastB(void)
{	ctxtEC_t far* ec;

	if((ec = ecValidateTOS()) == 0)
		return 0;

	do if(ec->ctxt_type == EC_TAG_BATCH)
			return ecData(ec, ctxtEC_Batch_t);
		else if(ec->ctxt_type < EC_FINAL_TAGS)
			break;
		else {
			ec += ec->ctxt_length + sizeof(ec);
			ec = (ctxtEC_t far*)_fnormalize(ec);
		}
	while(FP_SEG(ec) < ctxtECHighestSegm());

	return 0;
}
