/*
 *	Set environment variables
 *
 */

#include "../config.h"

#include <assert.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

static FLAG optC;

#pragma argsused
optScanFct(opt_set)
{ switch(ch) {
  case 'C': return optScanBool(optC);
  }
  optErr();
  return E_Useage;
}

int cmd_set(char *param)
{	char *value;

	if(leadOptions(&param, opt_set, 0) != E_None)
		return 1;

	switch(breakVarAssign(ctxtEnvironment, param, &value)) {
	case 1:			/* no equal sign */
		error_syntax(0);
		return 1;
	case 0:			/* displayed */
		return 0;
#ifdef DEBUG
	case 2: break;
	default:
		dprintf(("[SET: Invalid response from breakVarAssign()]\n"));
		return 1;
#endif
	}

	/* If the value is just blanks, it means to delete the value;
		but otherwise even leading and trailing spaces must be kept */
	if(is_empty(value))
		value = 0;

	return chgEnvCase(optC, param, value);
}
