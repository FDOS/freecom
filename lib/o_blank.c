/* $id$

	Output functions

	Add a pending pad space.

*/

#include "../config.h"

#include "out.h"

void oBlank(void)
{	oPadChars(OUT_MODE_PAD_BLANK, 1);
}
