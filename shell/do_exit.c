/* $id$

	Perform a E context (terminate)

	Just return 0
*/

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/context.h"
#include "../include/misc.h"

#pragma argsused
char *terminateShell(ctxtEC_t far * const ctxt)
{
	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_TERMINATE);

	cbreak;
	lflag_doExit = lflag_doCancel = lflag_doQuit = 0;
	return 0;
}
