/* $id$

	Output functions

	End the current line.

*/

#include "../config.h"

#include "out.h"

void oFlushNL(void)
{
	if(oColumn)
		oNL();
}
