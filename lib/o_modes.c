/* $id$

	Output functions

	Recalculate the dimensions of the screen.

*/

#include "../config.h"

#include <stdio.h>
#include <bios.h>

#include <portable.h>

#include "out.h"

void oGetScreenDimensions(void)
{	int mode;
	USEREGS

	/* if the current output device is a device & stdout -->
		use the BIOS video to acquire the screen dimensions from */
	if(oConsole()) {
		_AH = 0xf;				/* Get active mode & page */
		geninterrupt(0x10);
		mode = _AL;
		oPage = _BH;			/* destroys AX */
		switch(mode) {
		case 0: case 1: case 4: case 5:
			oColMax = 40;
			oRowMax = 25;
			break;
		case 2: case 3: case 6: case 7:
			oColMax = 80;
			oRowMax = 25;
			break;
		default:
			oRowMax = oColMax = 0;
			break;
		}
		if((biosequip() & 0x30) == 0) {
			/* Assume VGA card */
			oRowMax = oColMax = 0;
		}
		if(!oRowMax)
			oRowMax = peekw(0x40, 0x84) + 1;
		if(!oColMax)
			oColMax = peekw(0x40, 0x4a);
	} else {			/* file output */
		oRowMax = 0;	/* unlimited */
		oColMax = 80;
		oPage = -1;
	}
	dprintf(("[OUT: page: %d, columns: %d, rows: %d]\n"
	 , oPage, oColMax, oRowMax));
}
