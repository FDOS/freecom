/*
 *  ECHO.C - echo command.
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
 * 1999/04/27 ska
 * add: "ECHO." displays empty line
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "batch.h"
#include "strings.h"
#include "cmdline.h"

#pragma argsused
int cmd_echo(char *param)
{ int nostatus;
  /*
   * Perform ECHO command.
   */

  dprintf( ("[ECHO: %s]\n", param) );

  if(!param)
    param = "";
  else if(*param) {
    nostatus = !isspace(*param);
    param = ltrim(param + 1);
  } else nostatus = 0;

  if (stricmp(param, D_OFF) == 0)
    echo = 0;
  else if (stricmp(param, D_ON) == 0)
    echo = 1;
  else if (*param)
    puts(param);
  else if(nostatus)
    putchar('\n');
  else
    displayString(TEXT_MSG_ECHO_STATE, echo ? D_ON : D_OFF);

  return 0;
}
