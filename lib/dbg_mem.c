/*	$Id$

 *  Defines the functions only necessary while debugging is active

	This file bases on DEBUG.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.5  2006/09/11 00:07:22  blairdude
	Fixed compilation completely with Turbo C

	Revision 1.4  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.3  2001/06/11 20:45:38  skaus
	fix: dbg_printmem() #if must be #ifdef
	
	Revision 1.2  2001/06/11 20:33:37  skaus
	fix: dbg_printmem() if compiled in Large memory model, near is invalid
	
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

#ifdef DEBUG

#include <conio.h>	/* cputs */
#include <alloc.h>	/* heapcheck, coreleft, farcoreleft */
#include <stdlib.h>	/* abort */

#if sizeof(void*) != sizeof(void far*)
#define DISP_NEAR
#endif

void dbg_printmem (void) {
#ifdef DISP_NEAR
	static unsigned nearLast = 0;
#endif
	static unsigned long farLast = 0;

#ifdef DISP_NEAR
	unsigned nearThis;
#endif
	unsigned long farThis;

#if __TURBOC__ > 0x201
	switch(heapcheck()) {
	case _HEAPCORRUPT:
		cputs("HEAP CORRUPTED. Cannot proceed!\r\n");
		abort();
	case _HEAPEMPTY:
		cputs("NO HEAP. Cannot proceed!\r\n");
		abort();
	default:
		cputs("Unknown heapcheck() error. Cannot proceed!\r\n");
		abort();
	case _HEAPOK:
		break;
	}
#endif

#ifdef DISP_NEAR
	nearThis = coreleft();
#endif
	farThis = farcoreleft();

#ifdef DISP_NEAR
	dprintf(("[free memory: near=%6u far=%13lu]\n", nearThis, farThis));
	if(nearLast)
		dprintf(("[changed    : near=%6d far=%13ld]\n"
		 , nearThis - nearLast , farThis - farLast));
#else
	dprintf(("[free memory: far=%13lu]\n", farThis));
	if(farLast)
		dprintf(("[changed    : far=%13ld]\n", farThis - farLast));
#endif

#ifdef DISP_NEAR
	nearLast = nearThis;
#endif
	farLast = farThis;
}

#endif /* DEBUG */
