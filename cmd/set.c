/* $Id$
 *	Set environment variables
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <environ.h>
#include <nls_c.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#define promptBuffer 256

/* optC determines if env variable's name (param) is set using
   passed in case or uppercased, whereas upCaseValue determines
   if the value the user passes in (with the /P option) is
   stored as typed or uppercased. */
static int optC, promptUser, upCaseValue;

#pragma argsused
optScanFct(opt_set)
{ switch(ch) {
  /* case 'A': return optScanBool(optA); arithmetic expression */
  case 'C': return optScanBool(optC);
  case 'U': return optScanBool(upCaseValue);
#if 1
  case 'I': /* Display Information about current memory */
		printf("Size of environment segment: %u bytes; unused: %u\n"
		 , env_resize(0, 0), env_freeCount(env_glbSeg));
		return E_Other;
#endif
  case 'P': return optScanBool(promptUser);
  }
  optErr();
  return E_Useage;
}

int cmd_set(char *param)
{	char *value;
	char *promptBuf = 0;
	int ret;

	optC = promptUser = upCaseValue = 0;

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

	if(promptUser) {	/* -> Display the value, then read and assign */
		assert(value);
		fputs(value, stdout);
		promptBuf = malloc(promptBuffer);
		if(!promptBuf) {
			error_out_of_memory();
			return E_NoMem;
		}
		fgets(promptBuf, promptBuffer, stdin);
		if(cbreak) {
			free(promptBuf);
			return E_CBreak;
		}
		value = strchr(promptBuf, '\0');
		while(--value >= promptBuf && (*value == '\n' || *value == '\r'));
		value[1] = '\0';	/* strip trailing newlines */
		value = promptBuf;
	}

	/* If the value is just blanks, it means to delete the value;
		but otherwise even leading and trailing spaces must be kept */
	if(is_empty(value))
		value = 0;

	if (upCaseValue) StrUpr(value); /* set value as upper case, eg for if testing */

	ret = chgEnvCase(optC, param, value);
	free(promptBuf);
	return ret;
}
