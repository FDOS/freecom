/*	$id$
	$Locker$	$Name$	$State$

 	Check if the passed in pointer is really malloc'ed

	$Log$
	Revision 1.1.2.1  2001/08/02 21:54:49  skaus
	Update #13

	Revision 1.1.2.1  2001/07/07 20:37:17  skaus
	Update #6
	
 */

#include "../config.h"

#include <alloc.h>

#include "../include/debug.h"
#include "../include/misc.h"

void dbg_chkptr(void *p, char *fnam, int line)
{
	if(p) {
		if(heapchecknode(p) != _USEDENTRY)
			__assertfail( "Assertion failed: %s, file %s, line %d\n"
			 , "heapchecknode(p) == _USEDENTRY", fnam, line);
		if(isRegStr(p) >= 0)
			__assertfail( "Assertion failed: %s, file %s, line %d\n"
			 , "free'ing a regged string", fnam, line);
	}
}
