/*
	Declaration & settings for debugging

	The following functions expand to nothing, if DEBUG is not defined,
	otherwise they expand to the given statement.

	dprintf( a )	--> print	a	[ printf() enable while debugging ]
	dbg_printmem()	--> displays the memory and how it changed
*/

#ifndef __DEBUG__H
#define __DEBUG__H

#ifdef DEBUG
		/* DEBUG ENABLED */

#define dprintf(p)  if (fddebug) printf p
void dbg_printmem(void);

#else
		/* NO DEBUG */

#define dprintf(p)
#define dbg_printmem()

#endif	/* defined(DEBUG) */

#endif
