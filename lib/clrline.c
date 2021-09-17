/*	$Id$

 * clrcmdline
 	Clear a line, may use more than one physical line on screen

	This file bases on ENVIRON.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

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
#include <io.h>

#include "../include/misc.h"

void clrcmdline(char * const str, const int maxlen, int x, int y)
{
	size_t len = strlen(str);

	if(len > 0) {
		int step;

		for(step = len; step > 0; step--) {
			x--;
			if(x<0) {
				y--;
				x=SCREEN_COLS-1;
			}
		}

		assert(str);
		goxy(x, y);
		memset(str, ' ', len);
		dos_write(1, str, len);
		goxy(x, y);
		memset(str, 0, maxlen);
	}
}
