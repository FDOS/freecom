/* $id$

	Make the skeleton for a context

	Return:
		0: on failure, message already displayed
*/

#include "../config.h"

#include <assert.h>

#include <fmemory.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

ctxtEC_t far*ecMk(const enum ExecContext_Tags type, const unsigned length)
{	ctxtEC_t far* ec;
	unsigned ofs;

	ec = ctxtExecContext;

	assert(FP_SEG(ec) >= ctxtSegm);

		/* make it the maximum possible offset */
	ofs = FP_OFF(ec) + (FP_SEG(ec) - ctxtSegm) * 16;

	ec = MK_FP(ctxtSegm, ofs - length - sizeof(*ec));

	if(ofs < sizeof(*ec) || ofs - sizeof(*ec) <= length
	 || ecSetTOS(ec) != E_None) {
		error_context_out_of_memory();
		return 0;
	}

	ec->ctxt_type = type;
	ec->ctxt_length = length;

#ifdef DEBUG
	_fmemset(ecData(ec, char), '#', length);
#endif

	return ec;
}
