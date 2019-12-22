/* $Id$

	Tests if the string is constructed out of is_fnchar()
	characters only

*/

#include "../config.h"

#include <assert.h>

#include "../include/misc.h"
#ifdef DBCS
# include "mbcs.h"
#endif

int is_fnstr(const char * const s)
{	const char *p;

	assert(s);
#ifdef DBCS
	for(p = s; *p; p = CharNext(p))
#else
	for(p = s; *p; ++p)
#endif
		if(!is_fnchar(*p))
			return 0;

	return 1;
}
