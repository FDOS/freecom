/*	$id$
	$Locker$	$Name$	$State$

 *  Skip a quoted word, stop at end of string, any whitespace or
 *  at a given string (case-sensitive compare).

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

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
#include <ctype.h>
#include <string.h>

#include "../include/command.h"
#include "../include/cmdline.h"

char *skipqword(const char *pp, const char * const stop)
{ size_t len;
  int quote = 0;

  len = stop? strlen(stop): 0;

    if(*pp) do {
    if(quote) {
      if(quote == *pp)
        quote = 0;
    } else if(strchr(QUOTE_STR, *pp))
          quote = *pp;
    else if(isspace(*pp) || len && memcmp(pp, stop, len) == 0)
      break;
  } while(*++pp);

  return (char *) pp;		/* strip const */
}
