/*
 *  CALL.C - batch file call command.
 *
 *	Set the "call" flag, then proceed with the "normal" command
 *	execution.
 */

#include "../config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

static int optS = 0;		/* force to swap out FreeCOM during call */
static int optN = 0;		/* force to NOT swap (superceeds optS) */

#pragma argsused
optScanFct(opt_call)
{ switch(ch) {
  case 'S': return optScanBoolI(optS);
  case 'N': return optScanBoolI(optN);
  }
  optErr();
  return E_Useage;
}


int cmd_call(char *param)
{	int ec;

	if((ec = leadOptions(&param, opt_call, 0)) != E_None)
		return ec;

	if(param) {
		lflag_called = 1;
		if(optS)
			lflag_swap = TRUE;
		if(optN)
			lflag_swap = FALSE;
		parsecommandline(param);
		return 0;
	}

	error_missing_command("CALL");
	return 1;
}
