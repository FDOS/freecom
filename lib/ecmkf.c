/* $id$

	Make a new F context

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
{	char buf[EC_LENGTH_F];
	unsigned idVarname, idCmd, idPattern;
	int i;
	int rv;

	if((rv = ecMkS()) != E_None)
		return rv;

	assert(cmd);
	assert(varname);
	assert(params);
	assert(param);

	if((idVarname = ecPushString(varname)) == 0
	 || (idCmd = ecPushString(cmd)) == 0)
	 	return E_NoMem;

	i = params;
	while(i--) {
		assert(param[i]);
		if((idPattern = ecPushString(param[i])) == 0)
			return E_NoMem;
	}

		/* The '|' enables to replace idPattern later on */
	sprintf(buf, "%c%u %u|%u|", EC_TAG_FOR_FIRST, idVarname, idCmd, idPattern);

	return ctxtPush(CTXT_TAG_EXEC, buf);
}
