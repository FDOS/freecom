/*
 *  FOR.C - for command.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 19 Jul 1998 (Hans B Pufal) [HBP_001]
 *   Implementation of FOR
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 10-Aug-1998 ska
 * - added malloc() checking
 *
 * 1999/04/23 ska
 * bugfix: cmd_for(): missing out-of-memory check with batch_params()
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

int cmd_for(char *param)
{
	/*
	 * Perform FOR command.
	 *
	 * First check syntax is correct : FOR %v IN ( <list> ) DO <command>
	 *   v must be alphabetic, <command> must not be empty.
	 *
	 * If all is correct build a new bcontext structure which preserves
	 *   the necessary information so that readbatchline can expand
	 *   each the command prototype for each list element.
	 *
	 * You might look on a FOR as being a called batch file with one line
	 *   per list element.
	 */

	char *pp;
	char var;

	assert(param);

	/* Check that first element is % then an alpha char followed by space */

	if(*param != '%' || !isalpha(param[1]) || !isspace(param[2])) {
		displayString(TEXT_ERROR_BAD_VERABLE);
		return 1;
	}

	var = param[1];               /* Save FOR var name */
	param = ltrim(param + 2);   /* skip whitespaces */

	/* Check next element is 'IN' */

	if(!matchtok(param, "in")) {
		displayString(TEXT_ERROR_IN_MISSING);
		return 1;
	}

	/* Folowed by a '(', find also matching ')' */

	if(*param != '(' || 0 == (pp = strchr(param, ')'))) {
		displayString(TEXT_ERROR_MISSING_PARENTHESES);
		return 1;
	}

	*pp = '\0';
	param++;                      /* param now points at null terminated list */

	pp = ltrim(pp + 1);

	/* Check DO follows */

	if(!matchtok(pp, "do")) {
		displayString(TEXT_ERROR_DO_MISSING);
		return 1;
	}

	/* Check that command tail is not empty */

	if(*pp == '\0') {
		displayString(TEXT_ERROR_NO_COMMAND_AFTER_DO);
		return 1;
	}

	/* OK all is correct, build a bcontext.... */

	{
		struct bcontext *new = newBatchContext();

		if(!new)
			return 1;

		if((bc->forproto = strdup(pp)) == 0) {
			error_out_of_memory();
			exit_batch();   /* remove the newly created batch context */
			return 1;
		}

		if(!setBatchParams(param)) { /* Split out list */
			exit_batch();
			return 1;
		}

		bc->forvar = var;
		bc->shiftlevel = 1;     /* skip %0 <=> filename */
	}

	return 0;
}
