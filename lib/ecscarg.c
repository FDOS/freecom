/* $id$

	Fetch the arguments of an Execution context

	return:
		internal error code
*/

#include "../config.h"

#include <stdarg.h>
#include <stdio.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int ecScanArg(const char * const ctxt
	, const int num
	, const char * const fmt
	, ...)
{	va_list ap;

	va_start(ap, fmt);
	if(num != vsscanf(ctxt + 1, fmt, ap)) {
		error_context_corrupted();
		return E_Syntax;
	}
	return E_None;
}
