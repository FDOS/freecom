/*	$Id$

	Check if a key is pressed currently

	This file bases on INIT.C of FreeCOM v0.81 beta 1.

	$Log$
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

#include "../include/misc.h"

int keypressed(void)
{
  IREGS r;

#if defined(NEC98)
  r.r_ax = 0x0100;
  intrpt(0x18, &r);

  if (!(r.r_bx & 0x0100U))	/* bh=1 pressed bh=0 no key */
#elif defined(IBMPC)
  r.r_ax = 0x0100;
  intrpt(0x16, &r);

  /* Check the zero flag.  Z=0 means a key was pressed; Z=1 means no key */
  if (r.r_flags & 0x40)
#else
  /* DOS generic */
  r.r_ax = 0x0600;
  r.r_dx = 0xff;
  intrpt(0x21, &r);

  /* Check the zero flag.  Z=0 means a key was pressed; Z=1 means no key */
  if (r.r_flags & 0x40)
#endif
    return 0;
  else
    return 1;
}
