/* $id$

	Validate the current top of exec context stack and return
	a pointer to it

	Return:
		0: on error

*/

#include "../config.h"

#include <assert.h>

#include <mcb.h>
#include <suppl.h>

#include "../include/context.h"
#include "../err_fcts.h"

ctxtEC_t far *ecValidateTOS(void)
{	ctxtEC_t far *ec;
	unsigned len;

	assert(ctxtMain);
	ec = ctxtExecContext;
	assert(ec);

		/* absolute bottom of dynamic context */
	if(FP_SEG(ec) >= ctxtECHighestSegm())
		return 0;

		/* Does this context overflows the allocated bytes within
			the execution context stack? */
	len = ec->ctxt_length;
	if(addu(&len, FP_OFF(ec) + sizeof(*ec))
	 || (ctxtECHighestSegm() - FP_SEG(ec)) * 16 < ec->ctxt_length) {
		error_context_corrupted();
		return 0;
	}

	return ec;
}
