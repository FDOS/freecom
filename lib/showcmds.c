/*	$id$
	$Locker$	$Name$	$State$

 * show commands and options that are available.

	This file bases on INIT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/07/01 22:04:31  skaus
	Update #3

	Revision 1.1  2001/04/12 00:33:53  skaus
	chg: new structure
	chg: If DEBUG enabled, no available commands are displayed on startup
	fix: PTCHSIZE also patches min extra size to force to have this amount
	   of memory available on start
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling
	bugfix: COMMAND.COM A:\
	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
	add: command MEMORY
	bugfix: runExtension(): destroys command[-2]
	add: clean.bat
	add: localized CRITER strings
	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
		via STRINGS.LIB
	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
	add: fixstrs.c: prompts & symbolic keys
	add: fixstrs.c: backslash escape sequences
	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
	chg: splitted code apart into LIB\*.c and CMD\*.c
	bugfix: IF is now using error system & STRINGS to report errors
	add: CALL: /N
	
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../strings.h"

#pragma argsused
int showcmds(char *rest)
{
  struct CMD *cmdptr;
  int y;

  displayString(TEXT_MSG_SHOWCMD_INTERNAL_COMMANDS);
  y = 0;
  cmdptr = internalCommands;
  while (cmdptr->name)
  {
    if (++y == 8)
    {
      puts(cmdptr->name);
      y = 0;
    }
    else
      printf("%-10s", cmdptr->name);

    cmdptr++;
  }
  if (y != 0)
    putchar('\n');

  displayString(TEXT_MSG_SHOWCMD_FEATURES);
#ifdef FEATURE_ALIASES
	displayString(TEXT_SHOWCMD_FEATURE_ALIASES);
#endif
#ifdef FEATURE_ENHANCED_INPUT
	displayString(TEXT_SHOWCMD_FEATURE_ENHANCED_INPUT);
#endif
#ifdef FEATURE_HISTORY
	displayString(TEXT_SHOWCMD_FEATURE_HISTORY);
#endif
#ifdef FEATURE_FILENAME_COMPLETION
	displayString(TEXT_SHOWCMD_FEATURE_FILENAME_COMPLETION);
#endif
#ifdef FEATURE_SWAP_EXEC
	displayString(TEXT_SHOWCMD_FEATURE_SWAP_EXEC);
#endif
#ifdef FEATURE_CALL_LOGGING
	displayString(TEXT_SHOWCMD_FEATURE_CALL_LOGGING);
#endif
#ifdef FEATURE_LAST_DIR
	displayString(TEXT_SHOWCMD_FEATURE_LAST_DIR);
#endif
#ifdef FEATURE_KERNEL_SWAP_SHELL
	displayString(TEXT_SHOWCMD_FEATURE_KERNEL_SWAP_SHELL);
	if(F(swap) != ERROR && F(swap) == TRUE)
		displayString(TEXT_SHOWCMD_DEFAULT_TO_SWAP);
#endif
#ifdef FEATURE_INSTALLABLE_COMMANDS
	displayString(TEXT_SHOWCMD_FEATURE_INSTALLABLE_COMMANDS);
#endif
#ifdef FEATURE_NLS
	displayString(TEXT_SHOWCMD_FEATURE_NLS);
#endif
#ifdef FEATURE_DIRSTACK
	displayString(TEXT_SHOWCMD_FEATURE_DIRSTACK);
#endif
#ifdef DEBUG
	displayString(TEXT_SHOWCMD_FEATURE_DEBUG);
#endif
  putchar('\n');

  return 0;
}
