/*	$id$
	$Locker$	$Name$	$State$

 	chgEnvRemove():
 *  Insert/replace/delete a case-sensitive variable into the specified
 *	context
 *
 *  If value == NULL || *value == 0, the variable is deleted.
 *  If name == "", a body with an empty name is created.
 *
 *  Return: 0 on success;; !0 on failure

	$Log$
	Revision 1.1  2001/04/12 00:33:52  skaus
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

#include <environ.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../strings.h"

int chgEnvRemove(const char name[], const char value[])
{
	return chgEnv(name, (value && *value)? value: 0);
}
