/* $id$

	Skip any sequence of quoted characters.	

*/

#include "../config.h"

#include <string.h>

#include "../include/misc.h"

char *skipquote(const char * const line)
{	char *p;
	int ch;

	if((p = (char*)line) == 0)
		return 0;

	while((ch = *p) != 0 && is_quote(ch)) {
		char *q;
		if((q = strchr(p, ch)) == 0)	/* ignore missing quote */
			return strchr(p, '\0');
		p = q + 1;
	}

	return p;
}
