/* $id$

	Output functions

	Display a string from the resource.

*/

#include "../config.h"

#include "../include/misc.h"
#include "out.h"

void oID(const unsigned mode, const int size, const unsigned id)
{	char *str;

	if((str = getString(id)) != 0) {
		oBlockString(mode, size, str);
		myfree(str);
	}
}
