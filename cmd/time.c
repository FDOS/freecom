/*
 *  TIME.C - time internal command
 *
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

static FLAG noPrompt = 0;

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
		myfree(time);
		param = 0;
	}

	while(1) {
		if(!param) {
			if(noPrompt) return 0;

			displayString(TEXT_MSG_ENTER_TIME);
			fgets(s, sizeof(s), stdin);
			if (cbreak) {
				dprintf(("[TIME: Quit because of ^Break]\n"));
				return 1;
			}
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
