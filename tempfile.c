
/*
 *  TEMPFILE.C - tempfile() creates a unique temporary file
 *
 *
 *
 *  Comments:
 *    ALIAS.C is the only part using this function, this seems to be OK
 *    to let only _one_ temporary file be cached.
 *
 *  02/02/1996 (Oliver Mueller) --------------------------------------------
 *    started. There is a problem with the tmpfile() routine of Borland so
 *    I have implemented it new.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1998/10/27 ska
 * - changed to use tmpfn()
 * - changed to close file pointer itself in rmtmpfile()
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include "tempfile.h"
#include "command.h"

static char *tfn = 0;
static FILE *tfp = 0;

void rmtmpfile(void)
{
  if (tfn)
  {
    if (tfp)
    {
      fclose(tfp);
      tfp = 0;
    }
    remove(tfn);
    free(tfn);
    tfn = 0;
  }
}

FILE *tempfile(void)
{
  rmtmpfile();                  /* make sure the old temp file is already closed */

  tfn = tmpfn();
  if (!tfn)
  {
    return 0;
  }

  return tfp = fopen(tfn, "w+b");
}
