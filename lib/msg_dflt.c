/*	$id$
	$Locker$	$Name$	$State$

	Create default message or display Out-of-memory error onto screen

	This file bases on MESSAGES.C of FreeCOM v0.81 beta 1.

	$Log$
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
#include <stdlib.h>

#include "../include/misc.h"
#include "../strings.h"

char *defaultMessage(unsigned id)
{
  char *h;

  /* Allocate 5 bytes for the numerical ID itself */
  if(id != MSG_ERR_ID_OUTOFMEMORY
   && (h = malloc(sizeof(MSG_DFL_PATTERN) + 5)) != 0) {
    /* Got enough memory to allocate the stuff */
    sprintf(h, MSG_DFL_PATTERN, id);
    return h;
  }

  puts(MSG_DFL_OUTOFMEMORY);
  return 0;
}
