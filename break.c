/*
 *  BREAK.C - break command.
 *
 *  Comments:
 *
 * 14-Aug-1998 (John P Price)
 *   started.
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_BREAK

#include <assert.h>
#include <dos.h>

#include "command.h"
#include "strings.h"

int getbreak(void)
{
  struct REGPACK rp;

  rp.r_ax = 0x3300;
  intr(0x21, &rp);

  return rp.r_dx & 0xFF;
}

void setbreak(int OnOff)        /* Off = 0, On = 1 */
{
  struct REGPACK rp;

  rp.r_ax = 0x3301;
  rp.r_dx = OnOff;
  intr(0x21, &rp);
}

#pragma argsused
int cmd_break(char *param)
{
  switch(onoffStr(param)) {
  	default:
		displayString(TEXT_ERROR_ON_OR_OFF);
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_BREAK_STATE, getbreak() ? D_ON : D_OFF);
		break;
  	case OO_Off:	setbreak(0);	break;
  	case OO_On:		setbreak(1);	break;
	}

  return 0;
}

#endif
