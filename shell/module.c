/*
 *	Special FreeCOM defuinitions for the module management
 *
 * 2000/07/12 ska
 * started
 */

#include <string.h>
#include <mcb.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/debug.h"
#include "../include/module.h"
#include "../include/res.h"
#include "../include/misc.h"


#pragma argsused
static int loadModule(res_majorid_t major
	, res_minorid_t minor
	, unsigned long length
	, FILE* f
	, void *arg)
{	
	word segm;

	if(length > 0xfffful) {
		dprintf(("[CRITER resource too large.]\n"));
		return 0;
	}
	switch(minor) {
	case 0x00:		/* CRITER autofail */
		if(!autofail) break;
		goto loadMod;

	case 0x03:		/* CRITER code & strings */
		if(autofail) break;

	loadMod:
		/* If we reach here and swapOnExec == ERROR
			--> kernel does not allow swapping
			<-> each instance is loading its own context
			<-> the block is loaded is not detached from this instance */
		segm = allocPermBlk((unsigned)length, 0x82);
		if(!segm) {
			dprintf(("[Out of memory loading CRITER module.]\n"));
			return 0;
		}
		if(farread(MK_FP(segm, 0), (unsigned)length, f) != (unsigned)length) {
		  	dprintf(("[Error reading CRITER module.]\n"));
		  	freeSysBlk(segm);
			return 0;
		}
		*(word *)arg = segm;
		return 1;		/* stop enumRes() */

#ifdef DEBUG
	default:
		dprintf(("Error: Unsupported minor resource ID: %u\n"
		 , minor) );
		break;
#endif
	}

	return 0;		/* Other minor IDs are happily ignored */
}


/* Returns the pointer to the context pointer of the Critical Error
	handler and, if not loaded so far, loads it */
kswap_p modContext(void)
{	word segm;

	segm = 0;
	enumResources(RES_ID_CRITER, loadModule, &segm);
#ifdef DEBUG
	if(segm)
		dprintf(("[CRITER loaded to segment 0x%04x.]\n", segm));
#endif

	return (kswap_p)segm;
}
