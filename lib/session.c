/*	$Id$

	Save/restore the current session

	this copies the Context into near memory (if space allows)
	and restores it later

	$Log$
	Revision 1.5  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.4  2003/09/01 15:04:26  skaus
	bugfix: LOADHIGH: deallocating context if /L is present {Tom Ehlert}
	
	Revision 1.3  2002/11/12 21:56:17  skaus
	v0.83 Beta 52:
	
	Revision 1.2  2002/11/12 19:48:48  skaus
	bugfix: COPY: additional output to honor redirection {Eric Auer} [bugID #1313]
	
	Revision 1.1  2002/11/12 18:31:57  skaus
	add: save/restore session (swap context) {Tom Ehlert}
	
*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <suppl.h>
#include <mcb.h>
#include <environ.h>

#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../include/crossjmp.h"
#include "../include/misc.h"

static unsigned ctxtSavedSize;
static void *ctxtSavePtr;

void saveSession(void)
{
	if(ctxt && swapContext) {
		ctxtSavedSize = mcb_length(ctxt);
		dprintf(("[MEM: save context: %u bytes from 0x%04x]\n"
		 , ctxtSavedSize, ctxt)); 
		ctxtSavePtr = realloc(ctxtSavePtr, ctxtSavedSize);
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
	}
}
