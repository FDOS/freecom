/*
 *  ECHO.C - echo command.
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
 * 1999/04/27 ska
 * add: "ECHO." displays empty line
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "batch.h"
#include "strings.h"
#include "cmdline.h"

#pragma argsused
int cmd_echo(char *param)
{ int nostatus;
  /*
   * Perform ECHO command.
   */

  dprintf( ("[ECHO: %s]\n", param) );

	if(param && *param) {
		nostatus = !isspace(*param);
		param = ltrim(param + 1);
	} else nostatus = 0;

	switch(onoffStr(param)) {
	case OO_Null:	case OO_Empty:
	default:
	  if(nostatus)
		putchar('\n');
	  else
		displayString(TEXT_MSG_ECHO_STATE, echo ? D_ON : D_OFF);
	  break;

	case OO_Off:	echo = 0; break;
	case OO_On:		echo = 1; break;
    case OO_Other:
		puts(param);
	}

  return 0;
}
