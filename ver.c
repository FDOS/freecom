/*
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
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

#include "command.h"
#include "strings.h"
#include "cmdline.h"

const char shellver[] = "version 0.79cá KSS [" __DATE__ "]";
const char shellname[] = "FreeCom";

void short_version(void)
{
  printf("\n%s %s\n", shellname, shellver);
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

  if((argv = scanCmdline(rest, opt_ver, NULL, &argc, &opts)) == NULL)
    return 1;

  /* arguments are simply ignored */

  if(!opts)      /* Basic copyright notice */
    displayString(TEXT_MSG_VER_BASIC);
  else {
  if(optR)
      {                         /* version information */
        union REGS regs;
        regs.h.ah = 0x30;
        intdos(&regs, &regs);
        printf("DOS version %u.%u\n", regs.h.al, regs.h.ah);

        if (regs.h.bh == 0xfd)
        {
          if (regs.h.bl == 0xff)
          {
            printf("FreeDOS kernel (build 1933 or prior)\n",
                   regs.h.ch, regs.h.cl, regs.h.bl);
          }
          else
          {
            printf("FreeDOS kernel version %d.%d.%d\n",
                   regs.h.ch, regs.h.cl, regs.h.bl);
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
  }

  freep(argv);
  return ec;
}

#endif
