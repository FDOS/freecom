/* $Id$
 *  PAUSE.C - pause command.
 *
 * FREEDOS extension : If resteter is specified use that as the pause
 *   message.
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
 * 1999/05/03 ska
 * bugfix: '\n' issued after waiting for keypress
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/batch.h"
#include "../include/command.h"
#include "../strings.h"

int cmd_pause(char *param)
{

	if(param && *param)
		fputs(param, stdout);
	else
		displayString(TEXT_MSG_PAUSE);

	cgetchar();
	putchar('\n');

	return 0;
}
