/*
 *  PAUSE.C - pause command.
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
 * 1999/05/03 ska
 * bugfix: '\n' issued after waiting for keypress
 */

#include "config.h"

#ifdef INCLUDE_CMD_PAUSE

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "batch.h"
#include "strings.h"

#pragma argsused
int cmd_pause(char *param)
{
/*
 * Perform PAUSE command.
 *
 * FREEDOS extension : If parameter is specified use that as the pause
 *   message.
 */

  if (param && *param)
    fputs(param, stdout);
  else {
    char *p;

    if(( p = getMessage(TEXT_MSG_PAUSE)) == NULL) {
      error_out_of_memory();
      return E_NoMem;
    }

    fputs(p, stdout);
    free(p);
  }

  cgetchar();
  putchar('\n');

  return 0;
}

#endif
