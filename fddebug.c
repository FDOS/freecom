/*
 *  FDDEBUG.C - verify command.
 *
 *  Comments: Turn on/off the debug flag
 *
 * 30-Mar-2000 (John P Price <linux-guru@gcfl.net>)
 *   started.
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_FDDEBUG

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>

#include "command.h"
#include "strings.h"

#pragma argsused
int cmd_fddebug(char *param)
{
  if (!param || !*param)
    displayString(TEXT_MSG_FDDEBUG_STATE, fddebug ? D_ON : D_OFF);
  else if (stricmp(param, D_OFF) == 0)
    fddebug = 0;
  else if (stricmp(param, D_ON) == 0)
    fddebug = 1;
  else
    displayString(TEXT_ERROR_ON_OR_OFF);

  return 0;
}

#endif
