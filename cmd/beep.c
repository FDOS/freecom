/*
 *  BEEP.C - beep command.
 *
 */

#include "../config.h"

#include "../include/command.h"
#include "../include/misc.h"

#pragma argsused
int cmd_beep(char *param)
{
	beep();

	return 0;
}
