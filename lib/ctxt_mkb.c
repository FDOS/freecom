/* $Id$
	Creates a block of memory suiteable to hold the
	context.

	$Log$
	Revision 1.1  2002/11/12 18:31:57  skaus
	add: save/restore session (swap context) {Tom Ehlert}

*/

#include "../config.h"

#include <assert.h>

#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/crossjmp.h"
#include "../err_fcts.h"


void ctxtCreateMemBlock(unsigned length)
{
	env_resizeCtrl = ENV_USEUMB | ENV_ALLOWMOVE | ENV_LASTFIT;
	if((ctxt = env_create((unsigned)length)) == 0) {
		error_out_of_dos_memory();
		jmp_fatal(E_NoMem);
	}
	dprintf(("[MEM: create context: %u bytes @0x%04x]\n"
	 , (unsigned)length, ctxt));
}
