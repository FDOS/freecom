/*	$id$
	$Locker$	$Name$	$State$

 *  Insert/replace/delete an environment variable
 *
 *  If the variable already exists within the environment,
 *  its case is retained {So one can modify "winbootdir", for
 *  example}. If the variable shall be added to the environment,
 *  but is not (in any case-insensitive match) contained within the
 *  environment, the name is uppercased.
 *
 *  Return: 0 on success
 *      !0 on failure

	This file bases on ENVIRON.C of FreeCOM v0.81 beta 1.

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
#include <string.h>

#include <environ.h>
#include <nls_c.h>

#include "../include/context.h"
#include "../include/misc.h"

int chgEnvCase(const int keepCase, char var[], const char value[])
{
	assert(var);

	if(!keepCase && (env_matchVar(0, var) & 7) != 0)
	/* no match --> upcase variable */
		StrUpr(var);

	return chgEnv(var, value);
}
