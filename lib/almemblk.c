/*	$Id$

	Allocate a block of memory.
	Remove "Use UMB" if forceLow is true.

	$Log$
	Revision 1.1  2004/06/29 21:57:20  skaus
	fix: /LOW option

 */

#include "../config.h"

#include <assert.h>

#include <suppl.h>

#include "../include/command.h"
#include "../include/misc.h"

unsigned allocMemBlk(const unsigned size, const unsigned Xmode)
{	unsigned mode = Xmode;

	assert(size);
	if(forceLow)
		mode &= ~0x80;		/* Remove the "Use UMB" bit */
	return allocBlk(size, mode);
}
