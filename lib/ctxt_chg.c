/* $Id$

	Change the size of the local context. It will never shrink
	so that stored data is corrupted. If necessary, the context
	is relocated to another physical location.

*/

#include "../config.h"

#include <assert.h>
#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int ctxtChgSize(unsigned tosize)
{	ctxt_t new_context;

	assert(tosize);

	env_resizeCtrl = ENV_USEUMB | ENV_FIRSTFIT | ENV_ALLOWMOVE;
	new_context = ctxt? env_setsize(ctxt, tosize)	/* alter context */
	                  : env_create(tosize);			/* new context */

	if(new_context) {
		ctxt = new_context;
		return E_None;
	}

	error_out_of_memory();
	return E_NoMem;
}
