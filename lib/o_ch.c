/* $id$

	Output functions

	Display a single character.

*/

#include "../config.h"

#include <stdio.h>

#include "out.h"

void oCh(const int ch)
{	int x1, y1;
	int x2, y2;

	oFlush();		/* flush any pending requests */
	oGetCursor(&x1, &y1);
	putchar(ch);
	++oColumn;
	oGetCursor(&x2, &y2);

	if(oColMax && oColumn >= oColMax) {
		if(y1 == y2)		/* Issue the missing NL */
			putchar('\n');
		oColumn = 0;
		oRowIncrement();
	}
}
