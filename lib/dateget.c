/*	$id$
	$Locker$	$Name$	$State$

	Get DOS date (for Turbo C prior v3)

	This file bases on DATEFUNC.C of FreeCOM v0.81 beta 1.

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

#ifdef _NO__DOS_DATE
	/* Use this source only if the compiler does no provide this
		function of its own. */

#include <assert.h>

#include <assert.h>
#include <dos.h>
#include "../include/datefunc.h"

void _dos_getdate(struct dosdate_t *d)
{
  struct REGPACK r;

  assert(d);

  r.r_ax = 0x2A00;

  intr(0x21, &r);

  d->year = r.r_cx;
  d->month = r.r_dx >> 8;
  d->day = r.r_dx & 0xFF;
  d->dayofweek = r.r_ax & 0xFF;
}


#endif                          /* _NO_DATE */
