/* $Id$

	Search for an internal command and, if present,
	return the pointer to the entry within cmdtable.
	The hidden status is ignored for this function.

	The name is already uppercased.
 */

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/command.h"

struct IFCT *is_ifct(const char * const cname)
{	struct IFCT *fct;
	int len;

	if(cname) {
		len = strlen(cname) + 1;

		fct = internalFunctions;
		do if(memcmp(fct->name, cname, len) == 0)
			return fct;
		while((++fct)->name);
	}

	return 0;
}
