/*	$id$
	$Locker$	$Name$	$State$

	Returns the largest allocatable block of memory in local heap.

 */

#include "../config.h"

#include <stdlib.h>
#include <alloc.h>

#include "../include/debug.h"
#include "../include/misc.h"

unsigned my_coreleft(void)
{	unsigned len, probe;

	for(len = 0, probe = 0x8000; probe; probe >>= 1) {
		void *p;

		if((p = malloc(len | probe)) != 0) {
			len |= probe;
			myfree(p);
		}
	}

	return len;
}
