/*
 *  GOTO.C - goto command.
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
 * 28 Jul 1998 (Hans B Pufal) [HBP_003]
 *   Terminate label on first space character, use only first 8 chars of
 *   label string
 *
 * 10-Aug-1998 ska
 * - moved code into readbatchline() that automatically supports
 *   modifyable scripts & ^Break checks
 *
 * 1999/11/02 ska
 * bugfix: some DOS shells except a colon before the label
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "command.h"
#include "batch.h"
#include "strings.h"
#include "cmdline.h"

#pragma argsused
int cmd_goto(char *rest)
{
/*
 * Perform GOTO command.
 *
 * Only valid if batch file current.
 */

  char *tmp;

  while(bc && bc->forvar)   /* is FOR context */
    exit_batch();       /* remove it */

  if (bc == NULL)
  {
    /*!! not in batch error */

    return 1;
  }
  assert(rest);

  if(*rest == ':')    /* some old DOS shell excepts a colon */
    rest = skipdm(rest + 1);

  if (*rest == '\0')
  {
    displayString(TEXT_ERROR_NO_GOTO_LABEL);
    exit_batch();
    return 1;
  }

  tmp = skipwd(rest);
  *tmp = '\0';

  tmp = strdup(rest);
  if (!tmp)
  {
    error_out_of_memory();
    return 1;
  }

  /* Restart the batch file from the beginning */
  bc->brewind = 1;
  bc->blabel = tmp;

  return 0;
}
