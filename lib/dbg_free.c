/*	$id$
	$Locker$	$Name$	$State$

 	Implements a free() checking if the pointer really is malloc()'ed

	$Log$
	Revision 1.1.2.1  2001/07/07 20:37:17  skaus
	Update #6

 */

#include "../config.h"

#include <alloc.h>

#include "../include/debug.h"

void dbg_free(void *p, char *fnam, int line)
{
	if(p && heapchecknode(p) != _USEDENTRY)
		__assertfail( "Assertion failed: %s, file %s, line %d\n"
		 , "heapchecknode(p) == _USEDENTRY", fnam, line);
	free(p);
	heapfillfree('K');
}
