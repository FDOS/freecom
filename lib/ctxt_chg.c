/* $Id$

	Change the size of the local context. It will never shrink
	so that stored data is corrupted. If necessary, the context
	is relocated to another physical location.

*/

#include "../config.h"

#include <assert.h>

#include <environ.h>
#include <fmemory.h>
#include <mcb.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int ctxtChgSize(unsigned tosize)
{	ctxt_t new_context;

	assert(tosize);
	if(ctxtMain) {
		word tempSegm;
		unsigned curSize;
		/* Modify the size of an already existing dyn context */
		/* Algorithm:
			Because the usual dynamic context is allocted "last-fit",
			the current contents is duplicated into a temporary segment
			allocated with "first-fit" method, first.
		*/
		assert(ctxtp->ctxt_size);
		assert(ctxtMCB->mcb_size > 2);
		assert(ctxtMCB->mcb_size < 0x1000);
		assert(ctxtp->ctxt_size < ctxtMCB->mcb_size - 2);
		if(tosize < (curSize = mcb_length(ctxtMain))) {
			/* Check if the size does not shrink below the currently
				used space */
			while(curSize - tosize > env_freeCount(ctxtSegm))
				if(!ctxtSinglePurge()) {
					error_context_out_of_memory();
					dprintf(("[CTXT: Tried to shrink dyn "
					          "context below limit]"));
					return E_NoMem;
				}
		}
		tempSegm = DOSalloc(ctxtMCB->mcb_size, 0);	/* low mem, 1st fit */
		if(!tempSegm) {
			error_out_of_dos_memory();
			return E_NoMem;
		}
		ctxtPurgeCache();
		/* Copy the contents there, incl the MCB name */
		_fmemcpy(MK_FP(SEG2MCB(tempSegm), 8)
		 , MK_FP(SEG2MCB(ctxtMain), 8)
		 , ctxtMCB->mcb_size * 16 + 8);
		freeSysBlk(ctxtMain);
		ctxtMain = tempSegm;
	}

	new_context = allocSysBlk(tosize, 0x82);	/* last fit; high then low */
	if(new_context) {
		ctxtCB_t _seg *nctxtp = (ctxtCB_t _seg*)new_context;

redo:
		if(ctxtMain) {		/* Move the current contents there */
			unsigned ecLen;		/* #segm of exec context */

				/* Length of exec context stack area */
			ecLen = ctxtMCB->mcb_size - ctxtp->ctxt_size - 1;
			_fmemcpy(MK_FP(SEG2MCB(new_context), 8)		/* copy subMCB */
			 , MK_FP(SEG2MCB(ctxtMain), 8) , 16 + 8);	/* and mem name */
				/* all changes of the dyn ctxt memory block are 100%
					distributed into/from the strings area */
			nctxtp->ctxt_size = mcb_length(new_context) - ecLen - 1;
				/* copy the strings */
			_fmemcpy(MK_FP(new_context + 1, 0), MK_FP(ctxtSegm, 0)
			 , nctxtp->ctxt_size);
			_fmemcpy(MK_FP(new_context + 1 + nctxtp->ctxt_size, 0)
			 , MK_FP(ctxtExecContext, 0), ecLen);
			freeSysBlk(ctxtMain);
#ifdef DEBUG
			if(env_check(ctxtSegm)) {
				fputs("[CTXT: Dynamic context had been corrupted!\n"
				 , stderr);
				ctxtMain = 0;
				goto redo;
			}
#endif
		} else {
			/* Erase all the context */
			assert(sizeof(ctxtInitialEC) < 16);
			_fmemcpy(nctxtp, TO_FP(&ctxtInitialCB), sizeof(ctxtInitialCB));
			nctxtp->ctxt_size = mcb_length(new_context) - 2;
			nctxtp->ctxt_eOffs = 16 - sizeof(ctxtInitialEC);
			_fmemcpy(MK_FP(new_context + 2 + nctxtp->ctxt_size
			  , nctxtp->ctxt_eOffs)
			 , TO_FP(&ctxtInitialEC), sizeof(ctxtInitialEC));
		}
		ctxtMain = new_context;
		return E_None;
	}

	error_out_of_dos_memory();
	return E_NoMem;
}
