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
	byte far *echo;

	if(param && *param) {
		status = isspace(*param);
			/* Unlike other internal commands, puncation characters
				are not removed by ECHO */
		param = ltrimsp(param + 1);
	} else status = 1;

	echo = interactive
	 ? &F(dispPrompt)
	 : &F(echo);

	switch(onoffStr(param)) {
	case OO_Off:	*echo = 0; break;
	case OO_On:		*echo = 1; break;

	case OO_Null:	case OO_Empty:
	default:
		if(status) {
			fct(TEXT_MSG_ECHO_STATE, *echo ? D_ON : D_OFF);
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
