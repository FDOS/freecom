/*	$Id$

	Waits about N secs for a keypress.
	returns 0 if none else returns key pressed invisibly.

	This file bases on INIT.C of FreeCOM v0.81 beta 1.

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
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#include "../include/misc.h"

int cgetchar_timed(int secs)
{
	struct dostime_t start;
	struct dostime_t now;
	int num;
	static char space[1] = {' '};
	static char backspaces[2] = {'\b', '\b'};

	start.second = 60;	/* force decrement secs first time in loop */
	++secs;
	while(!keypressed()) {
		_dos_gettime(&now);
		if(now.second != start.second) {
			if(!--secs)
				return 0;
			memcpy(&start, &now, sizeof(now));
			printf("%d", secs);
			num = secs > 8 ? 2 : 1;
			if(secs == 9)
				dos_write(1, space, 1);
			dos_write(1, backspaces, num);
		}
		delay(100);
	}
		/* Because "keypressed()" returned true, getchar() does not block */
	return cgetchar();
}
