/* $id$

	Purge all external allocated memory holding cached items.

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"

void purgeMemory(void)
{
		/* Unload the message block if not loaded persistently */
		if(!gflag_persistentMSGs)
			unloadMsgs();
}
