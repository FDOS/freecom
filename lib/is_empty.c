/* $Id$

	Returns true if the strings is NULL, "" or consists of
	whitespaces only.

*/

#include "../config.h"

#include <ctype.h>

#include "../include/misc.h"

int is_empty(const char *s)
{	if(s) {
		--s;
		while(*++s)
			if(!isspace(*s))
				return 0;
	}
	return 1;
}
