/* $id$

	Output functions

	Flush any pending requests.

*/

#include "../config.h"

#include "out.h"

void oFlush(void)
{	int spaces, left;

	/* Apply padding spaces */
	if((spaces = oPadSpaces) != 0) {
		oPadSpaces = 0;
		for(; spaces && spaces >= (left = oColumnsLeft()); spaces -= left)
				oNL();

		while(spaces--)
			putchar(' ');
	}
}
