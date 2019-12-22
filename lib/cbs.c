/* $Id$

	Cut trailing backslashes

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/misc.h"
#ifdef DBCS
# include "mbcs.h"
#endif


void cutBackslash(char * const s)
{	char *p;
#ifdef DBCS
	char *s2;

	assert(s);
	p = s2 = (char *)s;
	while(*s2) {
		if (*s2 == '\\') p = (char *)s2;
		s2 = CharNext(s2);
	}
	if (p > (char *)s && p[1] == '\0' && p[-1] != ':')
		*p = '\0';
#else

	assert(s);
	p = strchr(s, '\0');

	/* take off trailing \ if any, but ONLY if dir is not the root dir */
	while(p > &s[1] && *--p == '\\' && p[-1] != ':')
		*p = '\0';
#endif
}
