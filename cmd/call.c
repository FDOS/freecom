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
#include "../err_fcts.h"

static int optS = 0;		/* force to swap out FreeCOM during call */
static int optN = 0;		/* force to NOT swap (superceeds optS) */

#pragma argsused
optScanFct(opt_call)
{ switch(ch) {
  case 'S': return optScanBool(optS);
  case 'N': return optScanBool(optN);
  }
  optErr();
  return E_Useage;
}


int cmd_call(char *param)
{	int ec;

	if((ec = leadOptions(&param, opt_call, 0)) != E_None)
		return ec;

	if(param) {
		called = 1;
		if(optS)
			swap = TRUE;
		if(optN)
			swap = FALSE;
		parsecommandline(param);
		return 0;
	}

	error_missing_command("CALL");
	return 1;
}
