/*
 * TRUENAME.C -- Truename Command (undocumented DOS?)
 *
 * 07/14/98 (Rob Lake)
 *  - started and tested fine
 *  - added stand alone debugging
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 28-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - changed _truename function so it does not return a pointer to a local
 *   variable.
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - _truename now sets errno
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_TRUENAME

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <errno.h>
#include <dir.h>

#include "command.h"

static int truename_(char * const dst, const char * const src)
{
  struct REGPACK rp;

  assert(dst);
  assert(src);

  rp.r_ds = FP_SEG(src);
  rp.r_si = FP_OFF(src);
  rp.r_es = FP_SEG(dst);
  rp.r_di = FP_OFF(dst);
  rp.r_ax = 0x6000;

  intr(0x21, &rp);

  if (rp.r_flags & 1)
  {
    return errno = rp.r_ax;
  }
  return 0;
}

#pragma argsused
int cmd_truename(char *rest)
{
  char p[MAXPATH * 2];

  assert(rest);

  if (truename_(p, *rest? rest: "."))
  {
    error_path_not_found();
    return 0;
  }

  puts(p);

  return 1;
}

#ifdef DEBUG_STANDALONE
int main(int argc, char **argv)
{
  cmd_truename(argv[1]);

  return 0;
}
#endif

#endif
