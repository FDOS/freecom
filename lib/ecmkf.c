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
	, const char * const varname
	, const char * const cmd)
{	ctxtEC_t far* ec;
	char far *p;
	unsigned length;
	int i;

	assert(cmd);
	assert(varname);
	assert(params);
	assert(param);

	length = strlen(cmd);
	if(addu(&length, strlen(varname))
	 || addu(&length, 2)) {
		error_context_out_of_memory();
		return E_NoMem;
	}

	for(i = 0; i < params; ++i) {
		char *p;

		assert(param[i]);
		if((p = esEncode(param[i])) == 0)
			return E_NoMem;
		chkPtr(param[i]);
		StrRepl(param[i], p);
		if(addu(&length, strlen(p) + 1)) {
			error_context_out_of_memory();
			return E_NoMem;
		}
	}

	if((ec = ecMk(EC_TAG_FOR_FIRST, length)) == 0)
		return E_NoMem;
	p = ecData(ec, char);
#define check(p)	\
	assert(FP_OFF(p) > FP_OFF(ecData(ec, char))	\
	 && FP_OFF(p) -  FP_OFF(ecData(ec, char)) < length)

	for(i = 0; i < params; ++i) {
		assert(param[i]);
		p = _fstpcpy(p, TO_FP(param[i])) + 1;
		check(p);
		p[-1] = ES_STRING_DELIM;
	}

	p[-1] = 0;
	p = _fstpcpy(p, TO_FP(varname)) + 1;
	check(p);

	p = _fstpcpy(p, TO_FP(cmd));
	check(p);
	assert(p + 1 == ecData(ec, char) + length);

	return E_None;
}
