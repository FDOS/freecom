/* $id$

	Output functions

	End the line.

*/

#include "../config.h"

#include "out.h"

void oNL(void)
{
	putchar('\n');
	oPadSpaces = oColumn = 0;
	oRowIncrement();
}
