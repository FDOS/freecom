/*
 *  ECHO.C - echo command.
 *
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

static int do_echo(char *param, void (*fct)(unsigned,...))
{	int status;

	if(param && *param) {
		status = isspace(*param);
			/* Unlike other internal commands, puncation characters
				are not removed by ECHO */
		param = ltrimsp(param + 1);
	} else status = 1;

	switch(onoffStr(param)) {
	case OO_On:		gflag_echo = 1; goto chkGFlgPrompt;
	case OO_Off:	gflag_echo = 0;
	chkGFlgPrompt:
		if(!gflag_batchlevel)
			gflag_dispPrompt = gflag_echo;
		break;

	case OO_Null:	case OO_Empty:
	default:
		if(status) {
			fct(TEXT_MSG_ECHO_STATE, gflag_echo ? D_ON : D_OFF);
			break;
		}
		/**FALL THROUGH**/
		param = "";

    case OO_Other:
		fct(TEXT_ECHO, param);
	}

	return 0;
}
int cmd_echo(char *param)
{	return do_echo(param, displayString);
}
int cmd_echoerr(char *param)
{	return do_echo(param, displayError);
}
