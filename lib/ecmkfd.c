/* $id$

	Make a silent and hidden c context executing FD

	Return:
		internal error code
*/

#include "../config.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../include/context.h"

int ecMkFD(const int jft, const int sft)
{	char buf[(sizeof(int) * 8 + 4) * 2];

	sprintf(buf, "%u %u", jft, sft);
	assert(strlen(buf) < sizeof(buf));

	return ecMkc("FD ", buf, (char*)0);
}
