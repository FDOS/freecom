/* $Id$
 *  GOTO.C - goto command.
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
 * 28 Jul 1998 (Hans B Pufal) [HBP_003]
 *   Terminate label on first space character, use only first 8 chars of
 *   label string
 *
 * 10-Aug-1998 ska
 * - moved code into readbatchline() that automatically supports
 *   modifyable scripts & ^Break checks
 *
 * 1999/11/02 ska
 * bugfix: some DOS shells except a colon before the label
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/batch.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

int cmd_goto(char *param)
{
	/*
	 * Perform GOTO command.
	 *
	 * Only valid if batch file current.
	 */

	char *tmp;

	while(bc && bc->forvar)   /* is FOR context */
		exit_batch();       /* remove it */

	if(bc == 0) {
	/*!! not in batch error */

		return 1;
	}
	assert(param);

	if(*param == ':')    /* some old DOS shell excepts a colon */
		param = skipdm(param + 1);

	if(*param == '\0') {
		error_goto_label();
		exit_batch();
		return 1;
	}

	tmp = skip_word(param);
	*tmp = '\0';

	tmp = strdup(param);
	if(!tmp) {
		error_out_of_memory();
		return 1;
	}

	/* Restart the batch file from the beginning */
	bc->brewind = 1;
	bc->blabel = tmp;

	return 0;
}
