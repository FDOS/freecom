/*	$Id$

	returns the dynamically allocated ASCII representation
	of the passed in date

	This file bases on NLS.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2006/09/05 01:44:33  blairdude
	Massive patches from Arkady that I'm way too lazy to sort through.  If anything happens, Arkady can answer for it.

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

#include <stdio.h>
#include <string.h>

#include "../include/misc.h"
#include "../include/nls.h"

char *nls_makedate (int mode, int year, int month, int day) {
        char buf[4 + 3 + sizeof(int) * 8 * 3];
        (void)mode;

#ifdef FEATURE_NLS
	refreshNLS();

	switch(nlsBuf->datefmt) {
	case 0:			/* mm/dd/yy */
		sprintf(buf, "%02u%s%02u%s%02u", month, nlsBuf->dateSep, day
		 , nlsBuf->dateSep, year);
		break;
	case 1:			/* dd/mm/yy */
		sprintf(buf, "%02u%s%02u%s%02u", day, nlsBuf->dateSep, month
		 , nlsBuf->dateSep, year);
		break;
	case 2:			/* yy/mm/dd */
		sprintf(buf, "%02u%s%02u%s%02u", year, nlsBuf->dateSep, month
		 , nlsBuf->dateSep, day);
		 
		break;
	}
#else
	sprintf(buf, "%02u-%02u-%02u", month, day, year);
#endif

	return strdup(buf);
}
