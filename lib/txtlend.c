/*	$Id$

 *	Returns the position of the first '\n' or '\0' character;
 *		or NULL if the line overflows the buffer.
 *	"overflow" means that no '\n' character was found and the line
 *	is completely full, so, if the very last line has no newline character,
 *	it is still considered "not overflowed".

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

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

#include "../include/misc.h"

char *textlineEnd(const char * const buf, const size_t buflen)
{	const char *p, *end;

	if(!buf)	return 0;
	end = buflen + (p = buf - 1);
	do { if(++p == end)		/* The very last byte of the buffer is
								hit ==> there ougth to be a '\0' there
								==> no '\n' AND no place for further
								character ==> overflow */
		return 0;
	} while(*p && *p != '\n');

	return (char *)p;		/* strip "const" */
}
