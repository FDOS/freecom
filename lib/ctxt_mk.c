/* $Id$

	Create local context

	The size is currently fixed to the default sizes of stuff
	located into the context: Aliases, History, Dirstack and a
	estimated size for batch nesting, shell and external program
	command line

*/

#include "../config.h"

#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/crossjmp.h"
#include "../err_fcts.h"

void ctxtCreate(void)
{	unsigned long length;		/* prevent overflow */

	length =
		MAX_FNAME			/* filename of shell */
		+ MAX_FNAME			/* filename of external program */
		+ EXEC_CONTEXT_MINIMUM_SIZE
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

	if(ctxtChgSize((unsigned)length) != E_None)
		jmp_fatal(E_NoMem);

	if(0
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
