/* $id$

	Make a silent C context.

	Return:
		internal error code
*/

#include "../config.h"

#include <stdlib.h>

#include <dynstr.h>

#include "ec.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

int ecMkV1C(const char * const str, ...)
{	char *line, *verbatim;
	va_list ap;
	int rv;

	if((verbatim = ecMkVerbatimStr(str)) == 0)
		return E_NoMem;
	if((line
	 = StrConcat(5, "%@VERBATIM(", verbatim, ")", str, verbatim)) == 0) {
		error_out_of_memory();
		myfree(verbatim);
		return E_NoMem;
	}

	myfree(verbatim);
	va_start(ap, str);
	rv = ecMkvcmd(lflag_echo? 0: EC_CMD_SILENT, line, ap);
	myfree(line);
	return rv;
}
