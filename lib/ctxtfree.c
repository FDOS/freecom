/* $id$

	Destroy (deallocate) the local context

*/

#include "../config.h"

#include "../include/context.h"
#include "../include/misc.h"


void ctxtDestroy(void)
{
	if(ctxtMain)
		freeSysBlk(ctxtMain);
	ctxtMain = 0;
}
