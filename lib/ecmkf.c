/* $id$

	Make a new F context

	The param[] array is updated with esEncoded() strings.

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <dynstr.h>
#include <fmemory.h>
#include <suppl.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int ecMkF(char ** const param
	, const int params
	, const char * const varnam
	, const char * const cmd)
{	ctxtEC_t far* ec;
	char far *p;
	unsigned length;
	int i;

	assert(cmd);
	assert(varnam);
	assert(param || !params);

	length = strlen(cmd);
	if(!addu(&length, strlen(varnam))
	 || !addu(&length, params)
	 || !addu(&length, 3)) {
		error_context_out_of_memory();
		return E_NoMem;
	}

	if((i = params) != 0) {
		char *p;

		while(i--) {
			assert(param[i]);
			if((p = esEncode(param[i])) == 0)
				return E_NoMem;
			chkPtr(param[i]);
			StrRepl(param[i], p);
			if(!addu(&length, strlen(p))) {
				error_context_out_of_memory();
				return E_NoMem;
			}
		}
	}

	if((ec = ecMk(EC_TAG_FOR_FIRST, length)) == 0)
		return E_NoMem;
	p = ecData(ec, char);

	for(i = 0; i < params; ++i) {
		assert(param[i]);
		p = _fstpcpy(p, TO_FP(param[i]));
		assert(_fnormalize(p) < _fnormalize(&ecData(ec, char)[length]));
	}

	p = _fstpcpy(p, TO_FP(varnam));
	assert(_fnormalize(p) < _fnormalize(&ecData(ec, char)[length]));

	p = _fstpcpy(p, TO_FP(cmd));
	assert(_fnormalize(p) == _fnormalize(&ecData(ec, char)[length - 1]));

	return E_None;
}
