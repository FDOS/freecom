/*
 *  DATE.C - date internal command
 *
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

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
  case 'T': return optScanBoolI(noPrompt);
  }
  optErr();
  return E_Useage;
}


static int my_setdate(const char *s)
{	struct dosdate_t d;

	switch(parsedate(s, &d)) {
	case E_None:		/* success -> set the date */
		_dos_setdate(&d);
		_dos_setdate(&d);		/* In WinNT the date is often one day +/- */
		/* fall through */
	case E_Empty:		/* empty line */
		return 1;		/* success */
	}

	return 0;			/* failure */
}

int cmd_date(char *param)
{
	char s[40];
	int ec;

	noPrompt = 0;

#ifdef FEATURE_NLS
	refreshNLS();
#endif

	if((ec = leadOptions(&param, opt_date, 0)) != E_None)
		return ec;

	if (!*param) {
		char *date;

		if((date = curDateLong()) == 0)
			return 1;		/* out of mem */

		displayString(TEXT_MSG_CURRENT_DATE, date);
		free(date);

		param = 0;
	}

	while(1)  {                    /*forever loop */
		if (!param) {
			if(noPrompt) return 0;

			displayString(TEXT_MSG_ENTER_DATE_AMERICAN
#ifdef FEATURE_NLS
			 + nlsBuf->datefmt, nlsBuf->dateSep, nlsBuf->dateSep
#else
			 , "-", "-"
#endif
			);
			fgets(s, sizeof(s), stdin);
			if(cbreak)
				return 1;
			param = s;
		}

		if(my_setdate(param))
			break;
		
		error_invalid_date();
		// force input the next time around.
		param = 0;
	}

	return 0;
}
