/* $Id$

	as calloc(), but runs error_out_of_memory() on error
*/

#include <stdlib.h>

#include "../err_fcts.h"
#include "../include/misc.h"

void *ecalloc(const size_t num, const size_t size)
{	void *h;

	if((h = calloc(num, size)) == 0)
		error_out_of_memory();

	return h;
}
