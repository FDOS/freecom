/*	$id$
	$Locker$	$Name$	$State$

	Returns the largest allocatable block of memory in the far heap.

 */

#include "../config.h"

#include <stdlib.h>
#include <alloc.h>

#include "../include/debug.h"
#include "../include/misc.h"

unsigned long my_farcoreleft(void)
{	unsigned long len, probe;

	for(len = 0, probe = 0x80000000l; probe; probe >>= 1) {
		void far*p;

		if((p = farmalloc(len | probe)) != 0) {
			len |= probe;
			farfree(p);
		}
	}

	return len;
}
