/* $id$

	Output functions

	Output a number of pad characters according mode.
	Spaces may be held back in order to be ignored if a NL follows
	immediately.

*/

#include "../config.h"

#include <dynstr.h>

#include "out.h"

#pragma argsused
void oPadChars(unsigned mode, int size)
{	/* Currently we do support spaces only */
	oColumn += size;
	oPadSpaces += size;
}
