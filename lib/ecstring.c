/* $id$

	Fetch an item from the STRING tag

	Return:
		0: on failure, error message already issued
		else: contents of string item (regged string!)

*/

#include "../config.h"

#include "../include/context.h"
#include "../include/misc.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

char *ecString(const unsigned id)
{	char *buf;

	switch(ctxtGet(0, CTXT_TAG_STRING, id, &buf)) {
	case 0:		/* OK */
		return regStr(buf);

	case 1:		/* no such item */
		dprintf(("[CTXT: Missing string #%u]\n", id));
		error_context_corrupted();
		break;

#ifdef DEBUG
	default:
		dprintf(("[CTXT: Invalid return value in ecString(): %u]\n"));
#endif

	case 2:		/* out of memory */
		error_out_of_memory();
		break;
	}

	return 0;
}
