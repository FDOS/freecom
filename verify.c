/*
 *  VERIFY.C - verify command.
 *
 *  Comments:
 *
 * 31 Jul 1998 (John P Price)
 *   started.
 *
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_VERIFY

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>

#include "command.h"
#include "strings.h"

#pragma argsused
int cmd_verify(char *param)
{
  if (!param || !*param)
    displayString(TEXT_MSG_VERIFY_STATE, getverify()? D_ON : D_OFF);
  else if (stricmp(param, D_OFF) == 0)
    setverify(0);
  else if (stricmp(param, D_ON) == 0)
    setverify(1);
  else
    displayString(TEXT_ERROR_ON_OR_OFF);

  return 0;
}

#endif
