/*	$id$
	$Locker$	$Name$	$State$

 * split - splits a line up into separate arguments, deliminators
 *      are spaces and /'s
 *  Each option is placed into one individual argument.
 *  The passed in string is preserved.

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
#include <stdlib.h>

#include "../include/cmdline.h"

static int addArg(char ***Xarg, int *argc, char *sBeg, char **sEnd)
{ char **arg;

  assert(Xarg);
  assert(argc);
  assert(sEnd);
  assert(sBeg);

    *sEnd = skip_word(sBeg);   /* find end of argument */

    /* Because *start != '\0' && !isargdelim(*start) ==> s != start */
    assert(*sEnd > sBeg);

      /* add new entry for new argument */
      if((arg = realloc(*Xarg, (*argc + 2) * sizeof(char *))) ==  0) {
        freep(*Xarg);
        return 1;
      }
      /* create new entry */
      if((arg[(*argc)++] = unquote(sBeg, *sEnd)) == 0) {
        freep(arg);
        return 1;
      }
      *Xarg = arg;

    return 0;
  }

char **split(char *s, int *args)
{
  char **arg,
   *start;
  int ac;

  assert(args);

  arg = malloc(sizeof(char *));
  if (!arg)
    return 0;
  ac = 0;

    /* skip to next argument */
  if(s) while (*(start = skipdm(s)) != '\0')
  {
    if(addArg(&arg, &ac, start, &s))
      return 0;
  }

  arg[*args = ac] = 0;
  return arg;
}
