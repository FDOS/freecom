/* $id$

	Output functions

	Return how many characters are left til line break

*/

#include "../config.h"

#include <limits.h>

#include "out.h"

int oColumnsLeft(void)
{
	if(!oColMax)
		return INT_MAX;
	return oColMax <= oColumn? 0: oColMax - oColumn;
}
