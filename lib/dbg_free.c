/*	$id$
	$Locker$	$Name$	$State$

 	Implements a free() checking if the pointer really is malloc()'ed

	$Log$
	Revision 1.1.2.2  2001/08/02 21:51:56  skaus
	Update #13

	Revision 1.1.2.1  2001/07/07 20:37:17  skaus
	Update #6
	
 */

#include "../config.h"

#include <alloc.h>

#include "../include/debug.h"

void dbg_free(void *p, char *fnam, int line)
{
	dbg_chkptr(p, fnam, line);
	free(p);
	chkHeap;
	heapfillfree('KK');
}
