/* $Id$

	Cut trailing backslashes

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/misc.h"


void cutBackslash(char * const s)
{	char *p;

	assert(s);

	p = strchr(s, '\0');

	/* take off trailing \ if any, but ONLY if dir is not the root dir */
	while(p > &s[1] && *--p == '\\' && p[-1] != ':')
		*p = '\0';
}
