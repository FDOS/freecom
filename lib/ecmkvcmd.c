/* $id$

	Make a C or c context.

	Return:
		internal error code
*/

#include "../config.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>
#include <fmemory.h>

#include "ec.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

int ecMkvcmd(int mode, const char * const str, va_list ap)
{	char *line = 0;
	char *p;
	ctxtEC_t far *ec;

	if(mode & EC_CMD_SILENT)
		if(!StrAppChr(line, '@')) {
			error_out_of_memory();
			myfree(line);
			return E_NoMem;
		}

	if(mode & EC_CMD_HIDDEN) {
		chkPtr(line);
		if(!StrCat(line, "::=")) {
			error_out_of_memory();
			myfree(line);
			return E_NoMem;
		}
	}

	chkPtr(line);
	if(!StrCat(line, str)) {
		error_out_of_memory();
		myfree(line);
		return E_NoMem;
	}

	while((p = va_arg(ap, char *)) != 0)  {
		chkPtr(line);
		if(!StrCat(line, p)) {
			error_out_of_memory();
			myfree(line);
			return E_NoMem;
		}
	}

	if(!line)			/* Don't make an empty context */
		return E_None;		/* Though -> it's successful */

	ec = ecMk((mode & EC_CMD_IGNORE_EXIT)
	  ? EC_TAG_COMMAND_IGNORE_EXIT
	  : EC_TAG_COMMAND
	 , strlen(line) + 1);

	if(ec) {
		_fstrcpy(ecData(ec, char), line);
		myfree(line);
		return E_None;
	}

	myfree(line);
	return E_NoMem;
}
