/*	$id$
	$Locker$	$Name$	$State$

 	chgCtxt():
 *  Insert/replace/delete a case-sensitive variable into the specified
 *	context
 *
 *  If value == NULL, the variable is deleted.
 *  If value == "", a variable with an empty body is created.
 *  If name == "", a body with an empty name is created.
 *
 *  Return: 0 on success;; !0 on failure

 	chgCtxtRemove():
 		as chgEnv(), but value == "" deletes the variable, too

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

#include <environ.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../strings.h"

int chgEnv(const char name[], const char value[])
{
	assert(name);

	/* return values 1 and 3 are OK */
	switch(env_change(0, name, value)) {
	case 2:			/* variable to delete not found <-> no problem */
	case 1: case 3:   /* var replaced | deleted | inserted ==> OK */
		return 0;
	case 0:       /* Cannot insert */
		if(!inInit)
			displayString(TEXT_ERROR_SET_ENV_VAR, name);
		return E_NoMem;
	default:
		displayString(TEXT_ERROR_NO_ENVIRONMENT);
		return E_Syntax;
	}

}
