/*
 *  GOTO.C - goto command.
 *
 *	Sets the "goto" flag, which gets checked only, if this is
 *	batch context currently.
 */

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <dynstr.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int cmd_goto(char *param)
{
	char *tmp;

	assert(param);

	if(*param == ':')    /* some old DOS shell excepts a colon */
		param = skipdm(param + 1);

	if(*param == '\0') {
		error_goto_label();
		return 1;
	}

	tmp = skip_word(param);
	*tmp = '\0';

	tmp = strdup(param);
	if(!tmp) {
		error_out_of_memory();
		return 1;
	}

	StrRepl(F(goto), tmp);

	return 0;
}
