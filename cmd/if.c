/*
 *  IF.C - if command.
 *
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int cmd_if(char *param)
{

#define X_EXEC 1

	int x_flag = 0;       /* when set cause 'then' clause to be exec'ed */
	int negate = 0;       /* NOT keyword present */

	char *pp;

	/* First check if param string begins with word 'not' */
	assert(param);

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

		if(findfirst(param, &f, 0) == 0)
			x_flag = X_EXEC;
	}

	/* Check for 'errorlevel' form */

	else if(matchtok(param, "errorlevel")) {
		int n = 0;

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

		if(gflag_errorlevel >= n)
			x_flag = X_EXEC;
	}

	/* Check that '==' is present, syntax error if not */
	else {
		size_t len;
		char *r;      /* right operand */

		pp = skipQuoteStr(param, "==");

		if(*pp != '=' || pp[1] != '=') {
			error_syntax(0);
			return 0;
		}

		*pp = '\0';     /* param[] points to the left operand */

		/* skip over the '==' and subsquent spaces and
			assign the end of the right operator to pp */
		pp = skipQuoteArg(r = ltrimcl(pp + 2));

		/*	now: param := beginning of the left operand
			r := beginning of the right operand
			pp := end of right operand
		*/

		rtrimcl(param);      /* ensure that spurious whitespaces are ignored */
		len = strlen(param);

		if((pp - r) == len
		 && memicmp(param, r, len) == 0) /* strings differ */
			x_flag = X_EXEC;
	}

	if(x_flag ^ negate)		/* perform the command */
		if(!*(pp = ltrimcl(pp)))
			error_missing_command("IF");
		else
			parsecommandline(pp);

	return 0;
}
