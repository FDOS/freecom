/* $Id$

	Tests if the string is constructed out of is_fnchar()
	characters only

*/

#include "../config.h"

#include <assert.h>

#include "../include/misc.h"

int is_fnstr(const char * const s)
{
	assert(s);
	return *skipfnam(s) == 0;
}
