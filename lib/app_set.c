/*	$Id$

	Enable DOS/APPEND if it was enabled before.

	void appendRestore(int state)

	Enable DOS/APPEND if state & 1

	$Log$
	Revision 1.1  2004/06/21 17:49:26  skaus
	fix: DIR: disable APPEND.EXE during DIR processing {Eduardo Almao}

 */

#include "../config.h"

#include <assert.h>
#include <dos.h>

#include "../include/misc.h"

void appendRestore(const int state)
{	if(state & 1) {		/* Re-enable DOS/APPEND */
		struct REGPACK r;

		r.r_ax = 0xb707;		/* Set APPEND function state */
		r.r_bx = state;
		intr(0x2f, &r);

		dprintf(("MUX-B7: set state 0x%04x]\n", state);
	}
}
