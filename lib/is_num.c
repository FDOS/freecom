/* $Id$

	Transform a decimal value into the binary form.
	Return:
		1: if all charcters of the string were used and the
			string is not empty
*/

#include "../config.h"

#include <assert.h>
#include <ctype.h>

#include "../include/misc.h"

int is_num(const char * const s, unsigned * const num)
{	unsigned n;
	const char *p;

	assert(num);

	if(!s || !*s)
		return 0;

	for(n = 0, p = s; *p; ++p)
		if(isdigit(*p)) {
			n *= 10;
			n += *p - '0';
		} else
			return 0;

	*num = n;
	return 1;
}
