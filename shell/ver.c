/* $Id$
 * VER.C
 *
 *  06/30/98 (Rob Lake)
 *      rewrote ver command to accept switches, now ver alone prints
 *      copyright notice only.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 30-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added text about where to send bug reports and get updates.
 *
 * 13-Sep-1998 (John P Price <linux-guru@gcfl.net>)
 * - change /r to print info. /d shows redistribution info now.
 *
 * 30-Jan-1999 (John P Price <linux-guru@gcfl.net>)
 * - changed /r to show FreeDOS kernel version
 *
 * 1-Apr-2000 0.79 Henry Voigt <Henry.Voigt@gmx.de>
 * bugfix: ver.c : options uninitilized
 *
 * 2000/06/22 ska
 *	new version: v0.79b
 *
 * 2000/12/10 ska
 *	new minor version: v0.80
 *
 * 2001/02/16 ska
 * bugfix: VER (without option) displays too much information
 * chg: using strings (except FreeCOM's own name)
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

#include "../include/command.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"
#include "../strings.h"

const char shellver[] = "0.84-pre"
#ifdef FEATURE_XMS_SWAP
	" XMS_Swap"
#endif
;
const char shelldate[] = __DATE__ " " __TIME__;
const char shellname[] = "FreeCom";

void short_version(void)
{
  printf("\n%s version %s [%s]\n", shellname, shellver, shelldate);
}

#ifdef INCLUDE_CMD_VER

static int optR, optW, optD, optC;

#pragma argsused
optScanFct(opt_ver)
{ switch(ch) {
  case 'R': return optScanBool(optR);
  case 'W': return optScanBool(optW);
  case 'D': return optScanBool(optD);
  case 'C': return optScanBool(optC);
  }
  optErr();
  return E_Useage;
}


/*
 *  display shell version info internal command.
 *
 *
 */
#pragma argsused
int cmd_ver(char *rest)
{
  char **argv;
  int argc, opts, ec = E_None;

  short_version();

	optR = optW = optD = optC = 0;

  if((argv = scanCmdline(rest, opt_ver, 0, &argc, &opts)) == 0)
    return 1;

  /* arguments are simply ignored */

  if(optR) {                         /* version information */
        union REGS regs;
        regs.h.ah = 0x30;
        intdos(&regs, &regs);
        displayString(TEXT_MSG_VER_DOS_VERSION, regs.h.al, regs.h.ah);

        if (regs.h.bh == 0xfd)
        {
          if (regs.h.bl == 0xff)
          {
          	displayString(TEXT_MSG_VER_EARLY_FREEDOS);
          }
          else
          {
            displayString(TEXT_MSG_VER_LATER_FREEDOS
             , regs.h.ch, regs.h.cl, regs.h.bl);
          }
        }
      }
      if (optW)
      {                         /* Warranty notice */
        displayString(TEXT_MSG_VER_WARRANTY);
      }
      if (optD)
      {                         /* Redistribution notice */
        displayString(TEXT_MSG_VER_REDISTRIBUTION);
      }
      if (optC)
      {                         /* Developer listing */
        displayString(TEXT_MSG_VER_DEVELOPERS);
      }

  freep(argv);
  return ec;
}

#endif
