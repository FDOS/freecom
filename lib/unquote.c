/*	$id$
	$Locker$	$Name$	$State$

 * removeQuotes - generates a new dynamically allocated string that
 *  contains no quotes (outer most quotes).
 *  The passed in string is preserved.
 *  Return: NULL on memory allocation problems

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/06/19 20:42:23  skaus
	Update #1

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
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>

#include "../include/cmdline.h"

char *unquote(const char *str, const char * const end)
{ char *h, *newStr;
  const char *q;
  size_t len;

  assert(str);
  assert(end);
  assert(end >= str);

  if((h = newStr = malloc(end - str + 1)) == 0)
    return 0;

  while((q = strpbrk(str, QUOTE_STR)) != 0 && q < end) {
  /* there is a quote at *q */
    /* copy the portion before q */
    memcpy(h, str, len = q++ - str);
    h += len;
    if((str = strchr(q, q[-1])) == 0 || str >= end) {
      /* no right quote */
      str = q;
      break;
    }
    /* copy the quoted portion */
    memcpy(h, q, len = str++ - q);
    h += len;
  }
  /* copy the remaining unquoted portion */
  memcpy(h, str, len = end - str);
  h[len] = '\0';
  return StrTrim(newStr);
}
