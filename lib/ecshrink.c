/* $id$

	Shrink the topmost context by diff bytes and retain the
	header of the stack item.

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/ierror.h"

int ecShrink(unsigned diff)
{	ctxtEC_t far *ec, far *nEc;
#ifndef NDEBUG
	int setTOSrv;
#endif

	if(diff) {
		ec = ecValidateTOS();
		assert(ec);

		assert(diff <= ec->ctxt_length);

		/* setTOS() does not modify the memory within the exec context
			therefore it's save to use the values of the current
			*ec later on */
#ifndef NDEBUG
		setTOSrv =
#endif
				ecSetTOS((ctxtEC_t far*)((byte far*)ec + diff));
#ifndef NDEBUG
		assert(setTOSrv == E_None);
#endif

		nEc = ctxtExecContext;
		nEc->ctxt_length = ec->ctxt_length - diff;
		nEc->ctxt_type = ec->ctxt_type;
	}

	return E_None;
}
