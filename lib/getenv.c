/*	$id$
	$Locker$	$Name$	$State$

 *  Return the value of the variable
 *
 *  The value will be overwritten the next time, getEnv() is called,
 *  because it will be duplicated into dynamic memory fromout the
 *  environment segment.
 *
 *  If the case-insensitive match is used, the var[] array is updated with
 *  the variable name that was retrieved.
 *
 *  Return: NULL, on failure

	This file bases on ENVIRON.C of FreeCOM v0.81 beta 1.

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

#include <environ.h>

#include "../include/misc.h"

char *getEnv(char var[])
{
  static char *lastVal = 0;

  assert(var);

  /* To have case-sensitive variable names, just delete the
     env_matchVar() call and the if() */
  /* To let var[] unaltered, use env_findAnyVar() instead of the
     combination of matchVar() & dupvar().
     However, the caller will know the correct variable name, if
     you don't. */
  if (env_matchVar(0, var) & 7) /* found? */
    return 0;                /* no match found */

  /* var now contains the correct variable name and we can be
     sure that's there */
  free(lastVal);

  return lastVal = dupvar(var);
}
