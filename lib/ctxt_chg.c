/* $Id$

	Change the size of the local context. It will never shrink
	so that stored data is corrupted. If necessary, the context
	is relocated to another physical location.

*/

#include "../config.h"

#include <assert.h>

#include <environ.h>
#include <fmemory.h>
#include <mcb.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

int ctxtChgSize(unsigned tosize)
{
	assert(tosize);

	purgeMemory();		/* remove all external memory */

	if(ctxtSegm) {
		ctxt_t tmpSegm;
		unsigned len;

		len = mcb_length(ctxtSegm);

		/* First duplicate block at the start of memory, then
			create the new block of memory, then remove the temp block */
		env_resizeCtrl = ENV_FIRSTFIT | ENV_ALLOWMOVE;
		if((tmpSegm = env_replace(ctxtSegm
		 , ENV_DELETE | ENV_COPY | ENV_CREATE | ENV_LIMITED)) != 0) {
			env_resizeCtrl = ENV_LASTFIT | ENV_ALLOWMOVE | ENV_USEUMB;
			if((ctxtSegm = env_replace(tmpSegm
			 , ENV_DELETE | ENV_COPY | ENV_CREATE, tosize)) != 0)
			 	return E_None;
			/* Try to recreate the original size */
			if((ctxtSegm = env_replace(tmpSegm
			 , ENV_DELETE | ENV_COPY | ENV_CREATE, len)) == 0)
			 	/* Argh, this won't make much fun */
			 	ctxtSegm = tmpSegm;
		}
	} else {
		/* Create a new segment */
		env_resizeCtrl = ENV_LASTFIT | ENV_ALLOWMOVE | ENV_USEUMB;
		if((ctxtSegm = env_create(tosize)) != 0)
			return E_None;
	}

	error_out_of_dos_memory();
	return E_NoMem;
}
