/* $Id$
 *  SHIFT.C - shift command.
 *
 * Only valid inside batch files.
 *
 * FREEDOS extension : optional parameter DOWN to allow shifting
 *   parameters backwards.
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/batch.h"
#include "../include/command.h"

int cmd_shift(char *param)
{
	struct bcontext *b = activeBatchContext();

	if(!b)
		/* not in batch - error */
		return 1;

	assert(param);
	if(stricmp(param, "down") == 0) {
		if(b->shiftlevel)
			b->shiftlevel--;
	} else                          /* shift up */
		b->shiftlevel++;

	return 0;
}
