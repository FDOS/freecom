/* $id$

	Output functions

	Get the current cursor position.

*/

#include "../config.h"

#include <portable.h>

#include "out.h"

void oGetCursor(int * const x, int * const y)
{
	if(oPage == -1 || oRowMax == 0)	{	/* unlimited */
		*y = oRowMax;
		*x = oColumn;
	} else {
		USEREGS

		_BH = oPage;
		_AH = 3;		/* Get cursor position & shape */
		geninterrupt(0x10);
		*y = _DH;
		*x = _DL;
	}
}
