/*	$id$
	$Locker$	$Name$	$State$

 * skip_word - skip a word / find next word delimiter
 *  word delimiters are whitespaces and non-leading option characters
 * skipdm - skip all word delimiters / find next non-word delimiter
 *  word delimiters are whitespaces only

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/07/16 20:28:38  skaus
	Update #9

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

#include "../include/cmdline.h"
#include "../include/misc.h"

/*
 * Find the next delimiter/non-delimiter within p
 *  Honor quotes and leading option characters
 */
static char *find(char *p, int delim)
{ int ch;
  int isopt;

  assert(p);

#define isdelim(ch) (isopt? isoptdelim(ch): isargdelim(ch))

  if((isopt = isoption(p)) != 0 && delim) {
    /* Assume the following example:
        p == "arg/opt"
      first find("arg/opt", 1) == find next delimiter
      returns pointer to "/opt"
      Now the process would call skip delimiters to reach the
      next argument --> find("/opt", 0) == find next non-delimiter
      will return "/opt", because '/' is delimiter only when
      searching for delimiters.
      Now the process would call skip non-delimiters to reach the
      end of the arg --> find("/opt", 1) == find next delimiter
      This time the leading optch's are part of the argument.
      ===> This is performed in this branch.

      If the string would be "/opt1/opt2", the call of
      find("/opt1/opt2", 1) == find next delimiter must stop
      at the second '/'.
    */
    while((ch = *++p) != 0 && isoptch(ch));
  }

  --p;
  while((ch = *(p = skipquote(p + 1))) != '\0'
   && ((delim && !(isdelim(ch) || isoptch(ch)))
    || (!delim && isdelim(ch))));

  return p;
#undef isdelim
}

char *skip_word(char *p)
{ return find(p, 1);
}

char *skipdm(char *p)
{ return find(p, 0);
}
