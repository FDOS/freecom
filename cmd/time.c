/* $Id$
 *  TIME.C - time internal command
 *
 *  Comments:
 *
 *  07/08/1998 (John P. Price)
 *    started.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1999/03/17 ska
 * bugfix: Detection of invalid time strings
 *  One can specify:
 *    ^\s*\d+.\d+(.(\d+(.\d*)?)?)?\s*([aApP]([mM]|\.[mM]\.)?\s*$
 *  If one specifies:
 *    1 number --> error
 *    2 numbers --> hour:minute, seconds & hundreds default to zero
 *    3 numbers --> hour:minute:seconds, hundreds defaults to zero
 *    4 numbers --> hour:minute:seconds.hundreds
 *  The numbers may be delimited by any character from the 7-bit ASCII set,
 *  which is printable, but not alphanumerical.
 *
 * 2001/02/08 ska
 * add: DATE /D and TIME /T
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/misc.h"
#include "../include/nls.h"
#include "../strings.h"

static int noPrompt = 0;

#pragma argsused
optScanFct(opt_date)
{ switch(ch) {
  case 'D':
  case 'T': return optScanBool(noPrompt);
  }
  optErr();
  return E_Useage;
}


static int my_settime(const char *s)
{	struct dostime_t t;

	switch(parsetime(s, &t)) {
	case E_None:		/* success -> set the date */
		_dos_settime(&t);
		/* fall through */
	case E_Empty:		/* empty line */
		return 1;		/* success */
	}

	return 0;			/* failure */
}

int cmd_time(char *param)
{
	char s[40];
	int ec;

	noPrompt = 0;

	if((ec = leadOptions(&param, opt_date, 0)) != E_None)
		return ec;

	if(!*param) {
		char *time;

		if((time = curTime()) == 0)
			return 1;

		displayString(TEXT_MSG_CURRENT_TIME, time);
		free(time);
		param = 0;
	}

	while(1) {
		if(!param) {
			if(noPrompt) return 0;

			displayString(TEXT_MSG_ENTER_TIME);
			fgets(s, sizeof(s), stdin);
			if (cbreak)
				return 1;
			param = s;
		}
		if(my_settime(param))
			break;
		error_invalid_time();
		/* force user interaction the next time around. */
		param = 0;
	}

	return 0;
}
