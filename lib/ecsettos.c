/* $id$

	Set a new top of exec context stack

	Return:
		E_NoMem: target TOS within dyn pseudo env <-> out-of-mem in dyn ctxt
		E_None: on success
*/

#include "../config.h"

#include <assert.h>

#include <environ.h>
#include <fmemory.h>
#include <mcb.h>

#include "../include/context.h"
#include "../include/ierror.h"

int ecSetTOS(const ctxtEC_t far *ec)
{
	ec = (const ctxtEC_t far *)_fnormalize(ec);

	assert(FP_OFF(ec) < 16);
	assert(ctxtSegm < FP_SEG(ec));
	assert(FP_SEG(ec) < ctxtECHighestSegm()
	 || FP_SEG(ec) == ctxtECHighestSegm() && FP_OFF(ec) == 0);

	if(FP_SEG(ec) >= ctxtECLowestSegm()) {
		ctxtp->ctxt_size = FP_SEG(ec) - ctxtSegm;
		ctxtp->ctxt_eOffs = FP_OFF(ec);
		return E_None;
	}
	return E_NoMem;
}
