/* $id$

	Perform a e context (keep running)

	As long as the stdin != EOF, push an I context, otherwise
	let this context be popped.

	Return:
		0: on error (pop this context --> terminate)
		else: cmdlineIgnore and I context pushed
*/

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../include/misc.h"

#pragma argsused
char *keepMeRunning(ctxtEC_t far * const ctxt)
{
	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_KEEP_RUNNING);

	doExit = doCancel = doQuit = 0;

	if(feof(stdin))
		return 0;

	return ecMkI() == E_None? cmdlineIgnore: 0;
}
