/* $Id$

	Tests if the string is constructed out of is_fnchar()
	characters only

*/

#include "../config.h"

#include <assert.h>

#include "../include/misc.h"

int is_fnstr(const char * const s)
{	const char *p;

	assert(s);
	for(p = s; *p; ++p)
		if(!is_fnchar(*p))
			return 0;

	return 1;
}
