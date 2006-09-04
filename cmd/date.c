/* $Id$
 *  DATE.C - date internal command
 *
 *  Comments:
 *
 *  08 Jul 1998 (John P. Price)
 *    started.
 *
 *  20 Jul 1998 (John P. Price)
 *  - corrected number of days for December from 30 to 31.
 *    (Thanx to Steffen Kaiser for bug report)
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 29-Jul-1998 (Rob Lake)
 * - fixed stand-alone mode.  Added Pacific C compatible dos_getdate functions
 *
 * 1999/03/17 ska
 * bugfix: detection for invalid date strings.
 *  Now the whole string must match: ^\s*\d+(.(\d+(.\d*)?)?)?\s*$
 *  The numbers can be delimited by any character from the 7-bit ASCII set,
 *  which is printable, but not alphanumerical.
 *  One need not specify all three parts (in which case the delimiter
 *  may be omitted, too). If one specify;
 *    1 number --> only the day changes
 *    2 numbers --> month/day; year remains unchanged
 *    3 numbers --> month/day/year
 *
 * 2001/02/08 ska
 * chg: two-digit year -> 2000 if less than 80
 * add: DATE /D and TIME /T
 *
 * 2001/02/14 ska
 * fix: years in range 80..199 are promoted to century 1900
 *		allows to parse dates created with year100 bug (by Arkady)
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
  case 'T': return optScanBool(noPrompt);
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
		/* force input the next time around. */
		param = 0;
	}

	return 0;
}
