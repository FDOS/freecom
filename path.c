/*
 *  PATH.C - path command.
 *
 *  Comments:
 *
 * 17 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 12-Aug-1998 ska
 * - added: SUPPL environment handling
 *   currently "PATH=" will delete the environment variable PATH;
 *   if an empty one shall be created, replace chgEnv1() by chgEnv()
 */

#include "config.h"

#ifdef INCLUDE_CMD_PATH

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "command.h"
#include "cmdline.h"
#include "strings.h"

#define PATHVAR    "PATH"

/*
 * changes the PATH env. var.
 */
#pragma argsused
int cmd_path(char *rest)
{

  if (!rest || !*rest)
  {
    displayString(TEXT_MSG_PATH, getEnv(PATHVAR));
    return 0;
  }

  if (*rest == '=')
    /* skip whitespaces */
    rest = ltrim(rest + 1);

  return chgEnv1(PATHVAR, rest);
}

#endif
