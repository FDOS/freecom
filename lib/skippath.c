/* $Id$

	skip complete path

	return the pointer of the next character that is no is_fnchar() and
	no path delimiter.

*/

#include "../config.h"

#include <assert.h>

#include "../include/misc.h"


char *skippath(const char * const path)
{	const char *p;

	assert(path);
	p = path - 1;
	while(is_fnchar(*++p) || is_pathdelim(*p));
	return (char *)p;
}
