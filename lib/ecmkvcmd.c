/* $id$

	Make a C or c context.

	Return:
		internal error code
*/

#include "../config.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <dynstr.h>
#include <fmemory.h>
#include <suppl.h>

#include "ec.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

int ecMkvcmd(int mode, const char * const str, va_list ap)
{	char *p;
	ctxtEC_t far *ec;
	unsigned len;
	va_list run;

	for(len = Strlen(str), run = ap; (p = va_arg(run, char *)) != 0;)  {
		if(addu(&len, strlen(p))) {
			error_long_internal_line();
			return E_NoMem;
		}
	}
	if(len >= INT_MAX - sizeof(ctxtEC_t)) {
		error_long_internal_line();
		return E_NoMem;
	}

	if(!len)			/* Don't make an empty context */
		return E_None;		/* Though -> it's successful */

	ec = ecMk(EC_TAG_COMMAND, len + sizeof(ctxtEC_Cmd_t));

	if(ec) {
		ctxtEC_Cmd_t far *cc;
		char far *q;

		cc = ecData(ec, ctxtEC_Cmd_t);
		cc->ec_flags = mode;
		q = cc->ec_cmd;
		if(str)
			q = _fstpcpy(q, TO_FP(str));
		while((p = va_arg(ap, char *)) != 0)
			q = _fstpcpy(q, TO_FP(p));
		assert(q - (char far*)cc == ec->ctxt_length - 1);
		return E_None;
	}

	return E_NoMem;
}
