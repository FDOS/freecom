/*
 *  DSTACK.C - Directory stack PUSHD/POPD support, compatible with 4/NDOS
 *
 *	PUSHD <dir>
 *
 * Pushes the current directory onto the stack.  If the length of
 * the directory string exceeds the remainer of the stack, it pops the first
 * entry, adjusts, and proceeds with the push.
 * Also: changes to the directory specified on the command line.
 */

#include "../config.h"

#include <stdlib.h>

#include "../include/context.h"
#include "../include/command.h"
#include "../include/misc.h"

int cmd_pushd(char *param)
{	char *curdir;

	if((curdir = cwd(0)) != 0) {
		int rc;

		rc = ctxtPush(CTXT_TAG_DIRSTACK, curdir);
		free(curdir);

		if(rc == E_None) {
			/* Change to directory specified on command line */
			if(param && *param)
				return cmd_cdd(param);
			return 0;
		}
	}
	return 1;
}
