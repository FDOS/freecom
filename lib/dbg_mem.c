/*	$id$
	$Locker$	$Name$	$State$

 *  Defines the functions only necessary while debugging is active

	This file bases on DEBUG.C of FreeCOM v0.81 beta 1.

	$Log$
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

#include "../include/debug.h"

#undef dbg_printmem
void dbg_printmem(void)
{	static unsigned nearLast = 0;
	static unsigned long farLast = 0;

	unsigned nearThis;
	unsigned long farThis;

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

	nearThis = coreleft();
	farThis = farcoreleft();

	dprintf(("[free memory: near=%6u far=%13lu]\n", nearThis, farThis));
	if(nearLast)
		dprintf(("[changed    : near=%6d far=%13ld]\n"
		 , nearThis - nearLast , farThis - farLast));

	nearLast = nearThis;
	farLast = farThis;
}
