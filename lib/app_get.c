/*	$Id$

	Enable DOS/APPEND if it was enabled before.

	int appendDisable()

	Disable DOS/APPEND and return the previous state;
	Return 0 if no APPEND is loaded.

	$Log$
	Revision 1.1  2004/06/21 17:49:25  skaus
	fix: DIR: disable APPEND.EXE during DIR processing {Eduardo Almao}

 */

#include "../config.h"

#include <assert.h>
#include <dos.h>

#include "../include/misc.h"

int appendDisable(void)
{	struct REGPACK r;

	r.r_ax = 0xb700;		/* APPEND installation check */
	intr(0x2f, &r);
	if(0xFF != (r.r_ax & 0xFF)) {	/* Not loaded */
#ifndef NDEBUG
		static int dispCnt = 1;
		if(r.r_ax & 0xFF)
			dprintf(("[INTERNAL: Unknown return value of MUX-B7-00: %u]\n", r.r_ax & 0xFF));
		if(--dispCnt <= 0) {
			dprintf(("[MUX-B7: No APPEND loaded]\n"));
			dispCnt = 10;
		}
#endif
		return 0;
	}

	/* APPEND is loaded */
	r.r_ax = 0xb706;		/* Get APPEND function state */
	intr(0x2f, &r);

	dprintf(("[MUX-B7: get state 0x%04x]\n", r.r_bx));

	return r.r_bx;
}
