/* $id$

	Returns the far address to the value of a specified tag item

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../include/context.h"

char *ctxtAddress(const Context_Tag tag, const unsigned num)
{	char *p = 0;

	ctxtGet(0, tag, num, &p);
	return p;
}
