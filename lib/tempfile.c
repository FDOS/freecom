/*	$id$
	$Locker$	$Name$	$State$

 *  TEMPFILE.C - tempfile() creates a unique temporary file
 *
 *
 *
 *  Comments:
 *    ALIAS.C is the only part using this function, this seems to be OK
 *    to let only _one_ temporary file be cached.
 *

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

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

#include <stdlib.h>
#include <stdio.h>

#include "../include/misc.h"
#include "../include/command.h"

static char *tfn = 0;
static FILE *tfp = 0;

void rmtmpfile(void)
{
	if (tfn) {
		if (tfp) {
			fclose(tfp);
			tfp = 0;
		}
		remove(tfn);
		free(tfn);
		tfn = 0;
	}
}

FILE *tempfile(void)
{
	rmtmpfile();  /* make sure the old temp file is already closed */

	tfn = tmpfn();
	if (!tfn)
		return 0;

	return tfp = fopen(tfn, "w+b");
}
