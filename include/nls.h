/*	$Id$

	Declarations to access the DOS NLS information

	$Log$
	Revision 1.1  2001/04/12 00:09:06  skaus
	chg: New structure
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

#ifndef __NLS_H
#define __NLS_H

#include <cntry.h>

extern Country *nlsBuf;		/* internally cached NLS info buffer */

void refreshNLS(void);		/* make sure the nlsBuf is valid */

/* The mode parameter is an OR combination of the following
	defines */
#define NLS_MAKE_SHORT_AMPM	1		/* AM/PM as single letters */


	/* Create a correctly constructed DATE string from the
		given parameters, the result is located in a dynamically
		allocated buffer */
char *nls_makedate(int mode, int year, int month, int day);
char *nls_maketime(int mode, int hour, int minute, int second, int fraction);

#endif
