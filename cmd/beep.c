/* $Id$
 *  BEEP.C - beep command.
 *
 *  Comments:
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
