/* $id$

	Make a new B context

	pos & lcnt default to 0

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"

int ecMkB(const char * const fullname)
{	ctxtEC_t far* ec;
	ctxtEC_Batch_t far *d;
	unsigned length;
#ifndef NDEBUG
	char far *q;
#endif

	assert(fullname);

	if((ec = ecMk(EC_TAG_BATCH, length = strlen(fullname) + sizeof(*d))) == 0)
		return E_NoMem;
	d = ecData(ec, ctxtEC_Batch_t);
	d->ec_pos = d->ec_lnum = 0;

#ifndef NDEBUG
	q =
#endif
	_fstpcpy(d->ec_fname, TO_FP(fullname));
	assert(_fnormalize(q) == _fnormalize(&ecData(ec, byte)[length - 1]));

	return E_None;
}
