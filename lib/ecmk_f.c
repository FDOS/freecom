/* $id$

	Make a new f context

	varname and cmd are inserted as offsets relative to ec's segment.

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>
#include <dynstr.h>
#include <suppl.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int ecMkf(const void * const ffblk
	, const char far* const varname
	, const char far* const cmd
	, const char * const prefix)
{	ctxtEC_t far* ec;
	ctxtEC_For_t far *p;
	unsigned length;
#ifndef NDEBUG
	char far *q;
#endif

	assert(cmd);
	assert(varname);
	assert(FP_SEG(varname) == FP_SEG(cmd));

	length = sizeof(ctxtEC_For_t);	/* incl '\0' of prefix */
	if(addu(&length, Strlen(prefix))) {
		error_context_out_of_memory();
		return E_NoMem;
	}

	if((ec = ecMk(EC_TAG_FOR_NEXT, length)) == 0)
		return E_NoMem;
	p = ecData(ec, ctxtEC_For_t);

	if(FP_SEG(ec) > FP_SEG(cmd)) {
		dprintf(("ecMkf(): cmd and varname are located at an inaccessable location\n"));
		ecPop();
		error_context_corrupted();
		return E_CorruptMemory;
	}
	_fmemcpy(p->ec_ffblk, TO_FP(ffblk), sizeof(p->ec_ffblk));
	assert(ctxtp->ctxt_size < 0x1000);
	p->ec_varname = FP_OFF(varname) + (FP_SEG(varname) - ctxtMain) * 16;
	p->ec_cmd = FP_OFF(cmd) + (FP_SEG(cmd) - ctxtMain) * 16;

	if(prefix) {
#ifndef NDEBUG
		q =
#endif
		_fstpcpy(p->ec_prefix, TO_FP(prefix));
		assert(FP_OFF(q) == FP_OFF(ecData(ec, byte)) + length - 1);
	} else {
		*p->ec_prefix = 0;
		assert(FP_OFF(p->ec_prefix) == FP_OFF(ecData(ec, byte)) + length - 1);
	}

	return E_None;
}
