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
#include "../include/misc.h"
#include "../err_fcts.h"

int ecMkvcmd(int mode, const char * const str, va_list ap)
{	char *p;
	char *buf, hbuf[EC_LENGTH_C];
	FLAG missingCmd;

	sprintf(hbuf, "%c%u ", EC_TAG_COMMAND, mode);
	if(mode & EC_CMD_FORCE_INTERNAL)
		strcat(hbuf, FORCE_INTERNAL_COMMAND_STRING);

	if((buf = StrConcat(2, hbuf, str)) == 0) {
		error_out_of_memory();
		return E_NoMem;
	}

	missingCmd = TRUE;
	while((p = va_arg(ap, char *)) != 0) if(*p) {
		missingCmd = FALSE;
		chkPtr(buf);
		if(!StrCat(buf, p)) {
			error_out_of_memory();
			myfree(buf);
			return E_NoMem;
		}
	}

	if(missingCmd) {		/* Don't make an empty context */
		myfree(buf);
		return E_None;		/* Though -> it's successful */
	}

	return ctxtPush(CTXT_TAG_EXEC, buf);
}
