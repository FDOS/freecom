/*	$id$
	$Locker$	$Name$	$State$

 *  Defines the functions only necessary while debugging is active

	This file bases on DEBUG.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2.2.2  2001/07/01 22:04:31  skaus
	Update #3

	Revision 1.2.2.1  2001/06/11 20:45:51  skaus
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

#include <conio.h>
#include <stdlib.h>
#include <alloc.h>

#include <suppl.h>

#include "../include/debug.h"
#include "../include/misc.h"

#if sizeof(void*) != sizeof(void far*)
#define DISP_NEAR
#endif


#undef dbg_printmem
void dbg_printmem(void)
{
#ifdef DISP_NEAR
	static unsigned nearLast = 0;
#endif
	static unsigned long farLast = 0;
	static word dosLast = 0;

#ifdef DISP_NEAR
	unsigned nearThis;
#endif
	unsigned long farThis;
	word dosThis;

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

#ifdef DISP_NEAR
	nearThis = my_coreleft();
#endif
	farThis = my_farcoreleft();
	dosThis = DOSalloc(0, 0x80 | 0x10);

#ifdef DISP_NEAR
	dprintf(("[free memory: near=%6u far=%13lu dos=%6u]\n"
	 , nearThis, farThis, dosThis));
	if(nearLast)
		dprintf(("[changed    : near=%6d far=%10ld dos=%6d]\n"
		 , nearThis - nearLast , farThis - farLast, dosThis - dosLast));
#else
	dprintf(("[free memory: far=%10lu dos=%6u]\n", farThis, dosThis));
	if(farLast)
		dprintf(("[changed    : far=%10ld dos=%6d]\n"
		 , farThis - farLast, dosThis - dosLast));
#endif

#ifdef DISP_NEAR
	nearLast = nearThis;
#endif
	farLast = farThis;
	dosLast = dosThis;
}
