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
{	char *p;

	assert(path);
	p = (char*)path - 1;
	do p = skipquote(p + 1);
	while(is_pathchar(*p));
	return p;
}
