/* $Id$

	Register a dynamically allocated string.
	Those strings are temporary strings with a short time to live;
	only a limited number of them are cached, the eldest is deallocated
	(FIFO).
*/

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include "../include/misc.h"

#if STRING_CACHE_ITEMS < 5
#error "A string cache of less than five items is too dangerous!"
#endif
static char *strCache[STRING_CACHE_ITEMS];

char *regStr(const char * const s)
{
	if(s) {
		free(strCache[0]);
		memmove(&strCache[0], &strCache[1]
		 , sizeof(strCache) - sizeof(strCache[0]));
		strCache[STRING_CACHE_ITEMS - 1] = (char*)s;
	}

	return (char*)s;
}
