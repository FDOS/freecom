/*
 *  SHIFT.C - shift command.
 *
 * Only valid inside batch files.
 *
 * FREEDOS extension : optional parameter DOWN to allow shifting
 *   parameters backwards.
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"

int cmd_shift(char *param)
{
	assert(param);
	if(matchtok(param, "down")) {
		if(gflag_shiftlevel)
			gflag_shiftlevel--;
	} else                          /* shift up */
		gflag_shiftlevel++;

	return 0;
}
