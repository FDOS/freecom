/* $Id$

	as realloc(), but runs error_out_of_memory() on error
*/

#include <stdlib.h>

#include "../err_fcts.h"
#include "../include/misc.h"

void *erealloc(void * const poi, const size_t size)
{	void *h;

	if((h = realloc(poi, size)) != 0)
		error_out_of_memory();

	return h;
}
