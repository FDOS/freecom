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

	if((buf = ctxtAddress(CTXT_TAG_STRING, id)) == 0) {
		dprintf(("[CTXT: Missing string #%u]\n", id));
		error_context_corrupted();
	}

	return buf;
}
