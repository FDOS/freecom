/* $Id$

	Create local context

	The size is currently fixed to the default sizes of stuff
	located into the context: Aliases, History, Dirstack and a
	estimated size for batch nesting, shell and external program
	command line

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../include/crossjmp.h"
#include "../err_fcts.h"

void ctxtCreate(void)
{	unsigned long length;

	length =
		MAX_FNAME			/* filename of shell */
		+ MAX_FNAME			/* filename of external program */
		+ CTXT_INFO(CTXT_TAG_BATCH, sizemax) * (MAX_FNAME + 10)
#ifdef FEATURE_LAST_DIR
		+ MAX_FNAME
#endif
#ifdef FEATURE_ALIASES
		+ CTXT_INFO(CTXT_TAG_ALIAS, sizemax)
#endif
#ifdef FEATURE_HISTORY
		+ CTXT_INFO(CTXT_TAG_HISTORY, sizemax)
#endif
#ifdef FEATURE_DIRSTACK
		+ CTXT_INFO(CTXT_TAG_DIRSTACK, sizemax)
#endif
	;

	if(length > CONTEXT_MAX_SIZE) {
		error_context_length(length, CONTEXT_MAX_SIZE);
		length = CONTEXT_MAX_SIZE;
	}
	ctxtCreateMemBlock((unsigned)length);
#if 0
	env_resizeCtrl = ENV_USEUMB | ENV_ALLOWMOVE | ENV_LASTFIT;
  if(forceLow)
	  env_resizeCtrl &= ~ENV_USEUMB;
	if((ctxt = env_create((unsigned)length)) == 0) {
		error_out_of_dos_memory();
		jmp_fatal(E_NoMem);
	}
	dprintf(("[MEM: create context: %u bytes @0x%04x]\n"
	 , (unsigned)length, ctxt));
#endif

	if(ctxtAddStatus(CTXT_TAG_BATCH)
#ifdef FEATURE_ALIASES
	 || ctxtAddStatus(CTXT_TAG_ALIAS)
#endif
#ifdef FEATURE_HISTORY
	 || ctxtAddStatus(CTXT_TAG_HISTORY)
#endif
#ifdef FEATURE_DIRSTACK
	 || ctxtAddStatus(CTXT_TAG_DIRSTACK)
#endif
	) {
		error_context_add_status();
		jmp_fatal(E_CorruptMemory);
	}
}
