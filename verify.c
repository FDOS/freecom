/*
 *  VERIFY.C - verify command.
 *
 *  Comments:
 *
 * 31 Jul 1998 (John P Price)
 *   started.
 *
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_VERIFY

#include <assert.h>
#include <dos.h>

#include "command.h"
#include "strings.h"

#pragma argsused
int cmd_verify(char *param)
{
  switch(onoffStr(param)) {
  	default:
		displayString(TEXT_ERROR_ON_OR_OFF);
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_VERIFY_STATE, getverify() ? D_ON : D_OFF);
		break;
  	case OO_Off:	setverify(0);	break;
  	case OO_On:		setverify(1);	break;
	}
  return 0;
}

#endif
