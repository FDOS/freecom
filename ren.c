/*
 * REN.C - rename command
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_RENAME

#include <assert.h>
#include <stdio.h>

#include "command.h"
#include "cmdline.h"

/*
 *  simple file rename internal command.
 *
 */
#pragma argsused
int cmd_rename(char *rest)
{ char **argv;
  int argc, opts, ec = E_None;

  if((argv = scanCmdline(rest, 0, 0, &argc, &opts)) == 0)
    return 1;

  if(argc < 2) {
    error_req_param_missing();
    ec = E_Useage;
  }
  else if(argc > 2) {
    error_too_many_parameters(rest);
    ec = E_Useage;
  }
  else if (rename(argv[0], argv[1]) != 0)
  {
    perror("rename");
    ec = E_Other;
  }

  freep(argv);
  return ec;
}

#endif
