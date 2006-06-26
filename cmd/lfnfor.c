/* $Id$
 *  LFNFOR.C - lfnfor command (MS-DOS 7.0).
 *
 *  Comments:
 *
 * 22-June-2006 (Blair Campbell)
 *   started.
 *
 */

#include "../config.h"

#include <string.h>

#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/misc.h"
#include "../strings.h"

unsigned char lfnfor = 0;
unsigned char lfncomplete = 0;

int cmd_lfnfor(char *param)
{
    if(strnicmp(param, "COMPLETE", 8) == 0) {
        switch(onoffStr(&param[8])) {
        default:
            error_on_off();
            return 1;
        case OO_Null:   case OO_Empty:
            displayString(TEXT_MSG_LFNFOR_COMPLETE_STATE, lfncomplete ?
                                                          D_ON : D_OFF );
            break;
        case OO_Off:    lfncomplete = 0; break;
        case OO_On:     lfncomplete = 1; break;
        }
    }
    else switch(onoffStr(param)) {
  	default:
		error_on_off();
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_LFNFOR_STATE, lfnfor ? D_ON : D_OFF);
		break;
  	case OO_Off:	lfnfor = 0;	break;
  	case OO_On:		lfnfor = 1;	break;
	}

	return 0;
}
