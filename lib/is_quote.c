/* $id$

	Return if the given character is a quote

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/misc.h"

int is_quote(const int c)
{	return c && strchr(QUOTE_STR, c);
}
