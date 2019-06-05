/*	$Id$

 * goxy
 *
 * move the cursor on the screen

	This file bases on CMDINPUT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.4  2006/06/14 05:25:06  blairdude
	goxy now uses the current video page for setting the cursor position

	Revision 1.3  2006/06/11 02:47:05  blairdude
	
	
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)
	
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
#include <dos.h>
#include <portable.h>

#include "../include/misc.h"

#if defined(NEC98)
void goxy(const unsigned char xo, const unsigned char yo)
{
	static char s[] = "\x1b" "[" "01;01H";
	unsigned char x = (xo <= 99) ? xo : 99;
	unsigned char y = (yo <= 99) ? yo : 99;
	s[2] = '0' + (y/10);
	s[3] = '0' + (y%10);
	s[5] = '0' + (x/10);
	s[6] = '0' + (x%10);
	cputs_int29(s);
}
#elif defined(IBMPC)
void goxy(const unsigned char x, const unsigned char y)
{
    USEREGS
    _AH = 0x0F;
    geninterrupt( 0x10 );
    _AH = 0x02;
    _DH = ( y - 1 );
    _DL = ( x - 1 );
    geninterrupt( 0x10 );
}
#endif

