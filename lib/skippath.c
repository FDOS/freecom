/* $Id$

	skip complete path

	return the pointer of the next character that is no is_fnchar() and
	no path delimiter.

*/

#include "../config.h"

#include <assert.h>

#include "../include/cmdline.h"
#include "../include/misc.h"


char *skippath(const char * const path)
{	const char *p;

	assert(path);
	if(is_quote(*path))
		return skipqword((char* const)path, (char*)0);
	p = path - 1;
	while(is_pathchar(*++p));
	return (char *)p;
}
