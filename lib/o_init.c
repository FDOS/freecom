/* $id$

	Output functions

		Initialize the internal states, e.g. forget any currently
		active ones.

*/

#include "../config.h"

#include <stdio.h>
#include <bios.h>

#include <portable.h>

#include "out.h"

unsigned oColumn;		/* current output column; == 0 -> none */
unsigned oPadSpaces;		/* how many padding spaces are pending */

int oPage;				/* Active console page;
									if == -1, no console is attached */
int oColMax;				/* Forced maximum column,
									if == 0, unlimited */
int oRowMax;				/* Forced maximum row;
									if == 0, unlimited */

void oInit(void)
{
	oPadSpaces = oColumn = 0;
	oGetScreenDimensions();
}
