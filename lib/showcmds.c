/*	$Id$

 * show commands and options that are available.

	This file bases on INIT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.7  2006/09/05 01:44:33  blairdude
	Massive patches from Arkady that I'm way too lazy to sort through.  If anything happens, Arkady can answer for it.

	Revision 1.6  2006/06/22 06:39:43  blairdude
	The FOR command now supports long filenames through LFNFOR (off by default), as in MS-DOS 7.0.  Since REN is buggy with long filenames at the moment, this change also ensures that it is compiled without long filename support.  the '?' command also now shows whether or not FreeCOM was compiled with long filename support or not (as it is becoming a major feature).
	
	Revision 1.5  2006/06/12 04:55:42  blairdude
	All putchar's now use outc which first flushes stdout and then uses write to write the character to the console.  Some potential bugs have been fixed ( Special thanks to Arkady for noticing them :-) ).  All CONIO dependencies have now been removed and replaced with size-optimized functions (for example, mycprintf, simply opens "CON" and directly writes to the console that way, and mywherex and mywherey use MK_FP to access memory and find the cursor position).  FreeCOM is now
	significantly smaller.
	
	Revision 1.4  2006/06/11 02:47:05  blairdude
	
	
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)
	
	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.2  2002/04/02 18:09:31  skaus
	add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
	
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

#include <stdio.h>

#include "../include/command.h"
#include "../include/kswap.h"
#include "../include/misc.h"
#include "../strings.h"

int showcmds (char * rest) {
  struct CMD *cmdptr;
  int y;

  (void)rest;
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
    outc('\n');

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
#ifdef FEATURE_LONG_FILENAMES
    displayString(TEXT_SHOWCMD_FEATURE_LONG_FILENAMES);
#endif
#ifdef FEATURE_KERNEL_SWAP_SHELL
	displayString(TEXT_SHOWCMD_FEATURE_KERNEL_SWAP_SHELL);
#define INIT_DISPLAY_DEFAULT_SWAP_VALUE
#endif
#ifdef FEATURE_XMS_SWAP
	displayString(TEXT_SHOWCMD_FEATURE_XMS_SWAP);
#define INIT_DISPLAY_DEFAULT_SWAP_VALUE
#endif
#ifdef INIT_DISPLAY_DEFAULT_SWAP_VALUE
	if(swapOnExec != ERROR && defaultToSwap == TRUE)
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
  outc('\n');

  return 0;
}
