/*	$Id$
	$Locker$	$Name$	$State$

	Save/restore the current session

	this copies the Context into near memory (if space allows)
	and restores it later

	$Log$
	Revision 1.2  2002/11/12 19:48:48  skaus
	bugfix: COPY: additional output to honor redirection {Eric Auer} [bugID #1313]

	Revision 1.1  2002/11/12 18:31:57  skaus
	add: save/restore session (swap context) {Tom Ehlert}
	
*/

#include "../config.h"

#include <assert.h>
//#include <stdlib.h>
#include <string.h>

#include <suppl.h>
#include <mcb.h>
#include <environ.h>

#include "..\err_fcts.h"
#include "..\include\command.h"
#include "..\include\context.h"
#include "..\include\crossjmp.h"

static unsigned ctxtSavedSize;
static void *ctxtSavePtr;

void saveSession(void)
{
	if(ctxt) {
		ctxtSavedSize = mcb_length(ctxt);
		dprintf(("[MEM: save context: %u bytes from 0x%04x]\n"
		 , ctxtSavedSize, ctxt)); 
		ctxtSavePtr = malloc(ctxtSavedSize);
		if(ctxtSavePtr) {
			_fmemcpy(ctxtSavePtr, MK_FP(ctxt,0), ctxtSavedSize);		
			DOSfree(ctxt);
#ifdef DEBUG
			ctxt = 0;
#endif
		}
	}
}
void restoreSession(void)
{
	assert(ctxt == 0);
	if(ctxtSavePtr) {
		dprintf(("[MEM: restore context: %u bytes]\n", ctxtSavedSize));
		ctxtCreateMemBlock(ctxtSavedSize);
		_fmemcpy( MK_FP(ctxt,0), ctxtSavePtr, ctxtSavedSize);		
		free(ctxtSavePtr);
		ctxtSavePtr = 0;
	}
}
