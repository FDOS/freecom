/* $id$

	Destroy (deallocate) the local context

*/

#include "../config.h"

#include "../include/context.h"
#include "../include/misc.h"


void ctxtDestroy(void)
{
	if(ctxtSegm)
		freeSysBlk(ctxtSegm);
	ctxtSegm = 0;
}
