/* $id$
	Extract the name of an internal command or alias
	for the beginning of a line.

	Return:
		0: on failure (out-of-mem --> message displayed)
		else: pointer to dynamically allocated name
			input pointer addresses 1st character, which is no
				part of the name
*/

#include "../config.h"

#include <string.h>

#include "../include/misc.h"
#include "../err_fcts.h"

char *getCmdName(const char ** const cp)
{	const char *p;
	char *h;
	int len;

	p = skipfnam(*cp);
	if(p == *cp || is_quote(*p))
		return 0;

	if((h = emalloc((len = p - *cp) + 1)) == 0)
		return 0;

	memcpy(h, *cp, len);
	h[len] = 0;
	*cp = p;

	return h;
}
