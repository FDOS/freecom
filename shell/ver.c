/*
 * VER.C
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

const char shellver[] = "version 0.83 Beta 24 expExec branch [" __DATE__ "]";
const char shellname[] = "FreeCom";

#pragma argsused
void short_version(char *param)
{
  printf("\n%s %s\n", shellname, shellver);
}

#ifdef INCLUDE_CMD_VER

static int optR, optW, optD, optC;

#pragma argsused
optScanFct(opt_ver)
{ switch(ch) {
  case 'R': return optScanBoolI(optR);
  case 'W': return optScanBoolI(optW);
  case 'D': return optScanBoolI(optD);
  case 'C': return optScanBoolI(optC);
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

  short_version(0);

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
