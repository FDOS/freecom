/*
 *  BREAK.C - break command.
 *
 */

#include "../config.h"

#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/misc.h"
#include "../strings.h"

int cmd_break(char *param)
{
	switch(onoffStr(param)) {
  	default:
		error_on_off();
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_BREAK_STATE, getbreak() ? D_ON : D_OFF);
		break;
  	case OO_Off:	setbreak(0);	break;
  	case OO_On:		setbreak(1);	break;
	}

	return 0;
}
