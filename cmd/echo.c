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

int cmd_echo(char *param)
{	int nostatus;
	FLAG *echo;

	if(param && *param) {
		nostatus = !isspace(*param);
			/* Unlike other internal commands, puncation characters
				are not removed by ECHO */
		param = ltrimsp(param + 1);
	} else nostatus = 0;

	echo = ctxtFlags.f_interactive
	 ? &ctxtFlags.f_dispPrompt
	 : &ctxtFlags.f_echo;

	switch(onoffStr(param)) {
	case OO_Null:	case OO_Empty:
	default:
	  if(nostatus)
		putchar('\n');
	  else
		displayString(TEXT_MSG_ECHO_STATE, *echo ? D_ON : D_OFF);
	  break;

	case OO_Off:	*echo = 0; break;
	case OO_On:		*echo = 1; break;
    case OO_Other:
		puts(param);
	}

	return 0;
}
