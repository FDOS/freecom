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

int ecScanArg(const char far * const ctxt
	, const int num
	, const char * const fmt
	, ...)
{	va_list ap;
	char *line;

	va_start(ap, fmt);
	if((line = regStr(edupstr(ctxt + 1))) == 0)
		return E_NoMem;
	if(num != vsscanf(line, fmt, ap)) {
		error_context_corrupted();
		return E_Syntax;
	}
	return E_None;
}
