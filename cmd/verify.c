/*
 *  VERIFY.C - verify command.
 *
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>

#include "../err_fcts.h"
#include "../include/command.h"
#include "../strings.h"

int cmd_verify(char *param)
{
  switch(onoffStr(param)) {
  	default:
		error_on_off();
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_VERIFY_STATE, getverify() ? D_ON : D_OFF);
		break;
  	case OO_Off:	setverify(0);	break;
  	case OO_On:		setverify(1);	break;
	}
  return 0;
}
