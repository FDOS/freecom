/*	$Id$

	Search for an external program and return it in a static buffer,
	which is overwritten the next time this function is called.

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.5  2004/09/13 18:59:40  skaus
	add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}

	Revision 1.4  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.3  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL
	
	Revision 1.2  2002/11/05 19:27:37  skaus
	bugfix: FreeCOM is to pass the fully-qualified path to DOS-4B-00
	
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
#include <errno.h>
#include <stdlib.h>

#include <dfn.h>

#include "../err_fcts.h"
#include "../include/misc.h"

char *find_which(const char * const fname)
{	char *p;
	static char *buf = 0;
	unsigned critCount;

	critEnableRepeatCheck();
	if(0 == (p = dfnsearch(fname, 0, 0))) {
		if(errno == ENOMEM) {
			error_out_of_memory();
		}
		critEndRepCheck();
		return 0;
	}

	free(buf);
	buf = abspath(p, 1);
	free(p);

	critEndRepCheck();
	return buf;
}
