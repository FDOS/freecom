/* $Id$

	Context status information 

*/

#include "../config.h"

#include "../include/context.h"

ctxt_info_t ctxt_info[] = {
	/* CTXT_TAG_ERROR */
	{ 0, 0, 0, 0 }				/* dummy entry */
	/* CTXT_TAG_FLAG */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_HISTORY */
	,{ HISTORY_DEFAULT_SIZE, 0, 0, 0 }
	/* CTXT_TAG_DIRSTACK */
	,{ DIRSTACK_DEFAULT_SIZE, 0, 0, 0 }
	/* CTXT_TAG_LASTDIR */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_SWAPINFO */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_ARG */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_IVAR */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_STRING */
	,{ (unsigned)-1, 0, 0, 0 }
	/* CTXT_TAG_EXEC */
	,{ (unsigned)-1, 0, 0, 0 }
/* pseudo entries */
	/* CTXT_TAG_ALIAS */
	,{ ALIAS_DEFAULT_SIZE, 0, 0, 0 }
};

#if sizeof(ctxt_info) / sizeof(ctxt_info_t) != (CTXT_LAST_TAG + 1)
#error "Not all tags have an ctxt_info entry"
#endif
