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

int ecShrink(int diff)
{	ctxtEC_t far *ec, far *nEc;

	if(diff) {
#ifndef NDEBUG
		int setTOSrv;
#endif
		ec = ecValidateTOS();
		assert(ec);

		assert(diff <= ec->ctxt_length);

#ifndef NDEBUG
		setTOSrv =
#endif
				ecSetTOS((ctxtEC_t far*)((byte far*)ec + diff));
#ifndef NDEBUG
		assert(setTOSrv == E_None);
#endif

		nEc = ecValidateTOS();
		assert(nEc);

		nEc->ctxt_length = ec->ctxt_length - diff;
		nEc->ctxt_type = ec->ctxt_type;
	}

	return E_None;
}
