/* $Id$
	Creates a block of memory suiteable to hold the
	context.

	$Log$
	Revision 1.2  2004/06/29 21:57:20  skaus
	fix: /LOW option

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
  if(forceLow)
	  env_resizeCtrl &= ~ENV_USEUMB;
	if((ctxt = env_create((unsigned)length)) == 0) {
		error_out_of_dos_memory();
		jmp_fatal(E_NoMem);
	}
	dprintf(("[MEM: create context: %u bytes @0x%04x]\n"
	 , (unsigned)length, ctxt));
}
