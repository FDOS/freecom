/*	$id$
	$Locker$	$Name$	$State$

 * goxy
 *
 * move the cursor on the screen

	This file bases on CMDINPUT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.2  2001/07/30 00:45:17  skaus
	Update #13 / Beta 27: plain dynamic context

	Revision 1.1.4.1  2001/07/25 20:17:28  skaus
	Update #12
	
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
#include <dos.h>

#include <portable.h>

#include "../include/misc.h"

void goxy(const unsigned char x, const unsigned char y)
{
	USEREGS

	assert(x > 0);
	assert(y > 0);


	_DX = ((y - 1) << 8) | (x - 1);
	_BH = 0;                /* video page 0 */
	_AH = 2;                /* set cursor position */
	geninterrupt(0x10);
}
