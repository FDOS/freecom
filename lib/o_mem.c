/* $id$

	Output functions

	Display a sequence of characters read from some memory location.

*/

#include "../config.h"

#include <stdio.h>

#include "../include/misc.h"
#include "out.h"

void oMem(const char * const line, const int size)
{	const char *p;
	int len, left;

	if((p = line) == 0 || (len = size) == 0)
		return;

	oFlush();		/* flush any pending requests */
	while(len > (left = oColumnsLeft())) {
		if(left-- > 1) {
			fwrite(p, 1, left, outStream);
			oColumn += left;
			len -= left;
			p += left;
		}
		/* display a single character causing any necessary line wraps */
		oCh(*p++);
		--len;
	}

	fwrite(p, 1, len, outStream);
	oColumn += len;
}
