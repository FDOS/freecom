/*
 *  DSTACK.C - Directory stack PUSHD/POPD support, compatible with 4/NDOS
 *
 *	POPD [*]
 *
 * Pops a directory off the stack, and sets it as cwd.
 * Accepts the parameter '*', causing it to wipe the stack.
 */

#include "../config.h"

#include <stdlib.h>

#include "../include/context.h"
#include "../include/command.h"
#include "../include/misc.h"
#include "../strings.h"

int cmd_popd(char *param)
{
	if(!param || *param != '*') {
		char *todir;

		if(ctxtPop(CTXT_TAG_DIRSTACK, &todir)) {
			int rc;

			rc = cmd_cdd(todir);
			free(todir);
			return rc;
		}

		displayString(TEXT_DIRSTACK_EMPTY);
	}

		/* Wipe the dirstack */
	ctxtClear(CTXT_TAG_DIRSTACK);

	return 0;
}
