/*
 *  FDDEBUG.C - verify command.
 *
 *  Comments: Turn on/off the debug flag
 *
 * 30-Mar-2000 (John P Price <linux-guru@gcfl.net>)
 *   started.
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_FDDEBUG

#include <assert.h>
#include <dos.h>

#include "command.h"
#include "strings.h"

int cmd_fddebug(char *param)
{
	switch(onoffStr(param)) {
  	default:
		displayString(TEXT_ERROR_ON_OR_OFF);
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_FDDEBUG_STATE, fddebug ? D_ON : D_OFF);
		break;
  	case OO_Off:	fddebug = 0;	break;
  	case OO_On:		fddebug = 1;	break;
	}
  return 0;
}

#endif
