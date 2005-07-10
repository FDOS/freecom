/* $Id$
 *  IF.C - if command.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1999/11/04 ska
 * + bugfix: if: case-insensitive compare  in "==" format
 * + bugfix: if: added support for quoted operands of "==" format
 * + add: if: detailed error messages
 * + bugfix: if: keyword "EXIST" misspelled
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/batch.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"

static int ignore_case = 0;

#pragma argsused
optScanFct(opt_if)
{ switch(ch) {
  case 'I': return optScanBool(ignore_case);
  }
  optErr();
  return E_Useage;
}


int cmd_if(char *param)
{

#define X_EXEC 1

	char *pp;
	int ec;

	int x_flag = 0;       /* when set cause 'then' clause to be exec'ed */
	int negate = 0;       /* NOT keyword present */

	ignore_case = 0;      /* /I option, case insensitive compare */


	/* First check if param exists */
	assert(param);

	/* check for options */
	if((ec = leadOptions(&param, opt_if, 0)) != E_None)
		return ec;

	/* next check if param string begins with word 'not' */
	if(matchtok(param, "not"))
		negate = X_EXEC;            /* Remember 'NOT' */

	/* Check for 'exist' form */

	if(matchtok(param, "exist")) {
		struct ffblk f;

		if(!*param) {
			/* syntax error */
			error_if_exist();
			return 0;
		}

		pp = skip_word(param);
		*pp++ = '\0';

		if(FINDFIRST(param, &f, FA_NORMAL|FA_ARCH|FA_SYSTEM|FA_RDONLY|FA_HIDDEN) == 0)
			x_flag = X_EXEC;
	}

	/* Check for 'errorlevel' form */

	else if(matchtok(param, "errorlevel")) {
		int n = 0;

#if 0
		if(!isdigit(*param)) {
			error_if_errorlevel();
			return 0;
		}

		pp = param;
		do  n = n * 10 + (*pp - '0');
		while (isdigit(*++pp));

		if(*pp && !isargdelim(*pp)) {
			error_if_errorlevel_number();
			return 0;
		}
#else
		/* Add this COMMAND bug as someone tries to use:
			IF ERRORLEVEL H<upper-case_letter>
			-or-
			IF ERRORLEVEL x<lower-case_letter>

			to match the errorlevel against drive letters.
			NOT supported by 4dos or WinNT.

			HA --> maps to errorlevel 1
			xa --> same

			HB & xb --> to 2
			a.s.o.
		*/

		if(!*param) {
			error_if_errorlevel();
			return 0;
		}
		pp = param;
		do  n = n * 10 + (*pp - '0');
		while(*++pp && !isargdelim(*pp));
		n &= 255;
		dprintf( ("IF: checking for ERRORLEVEL >= %u\n", n) );
#endif

		if(errorlevel >= n)
			x_flag = X_EXEC;
	}

	/* Check that '==' is present, syntax error if not */
	else {
		size_t len;
		char *r;      /* right operand */

		pp = skipqword(param, "==");

		if(*pp != '=' || pp[1] != '=') {
			error_syntax(0);
			return 0;
		}

		*pp = '\0';     /* param[] points to the left operand */

		/* skip over the '==' and subsquent spaces and
			assign the end of the right operator to pp */
		pp = skipqword(r = ltrimcl(pp + 2), 0);

		/*	now: param := beginning of the left operand
			r := beginning of the right operand
			pp := end of right operand
		*/

		rtrimcl(param);      /* ensure that spurious whitespaces are ignored */
		len = strlen(param);

		/* check if strings differ */
		if ( ((pp - r) == len) &&
		     ((ignore_case && strncmpi(param, r, len) == 0) ||
		      (memcmp(param, r, len) == 0)) )
			x_flag = X_EXEC;
	}

	if(x_flag ^ negate)		/* perform the command */
		if(!*(pp = ltrimcl(pp)))
			error_if_command();
		else
			parsecommandline(pp, FALSE);

	return 0;
}
