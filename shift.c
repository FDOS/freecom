/*
 *  SHIFT.C - shift command.
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
 *
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "batch.h"

#pragma argsused
int cmd_shift(char *param)
{
/*
 *  Perform the SHIFT command.
 *
 * Only valid inside batch files.
 *
 * FREEDOS extension : optional parameter DOWN to allow shifting
 *   parameters backwards.
 */

  if (bc == 0)
  {
    /*!! not in batch - error */

    return 1;
  }

  assert(param);
  if (stricmp(param, "down") == 0)
  {
    if (bc->shiftlevel)
      bc->shiftlevel--;
  }
  else                          /* shift up */
    bc->shiftlevel++;

  return 0;
}
