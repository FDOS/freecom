/* $Id$
 *	Module implementation for CB_CATCH
 *
 * 2000/07/12 ska
 * started
 */

#include <stdarg.h>
#include <dos.h>
#include <mcb.h>

#include "config.h"
#include "debug.h"
#include "module.h"
typedef void interrupt (*isr)();

static MOD_LOADSTATE_STRUCT(mod_state_cbreak_t)
} state;

#include "cb_catch.ver"		/* module's version */
#include "dmy_cbrk.inc"			/* dummy ^Break handler */

static const char modToken[] = "CBREAK";


int modCBreak(mod_state_t action, ...)
{	va_list ap;
	word segm;
	MIB far *mib;

	va_start(ap, action);

	switch(action) {
	case MOD_ATTACH:	/* try to attach to specified already loaded module */
		if(state.mode != MOD_ATTACHED) {
			segm = va_arg(ap, word);
			mib = MCBtoMIB(segm);
			if(mib->mib_majorID == RES_ID_CBREAK
			  /* && mib->mib_minorID == *			not used */
			 && mib->mib_version == MOD_VERSION)
				/* may attach to this module */
				attachSimpleModule(segm, (mod_load_state_t*)&state, modToken);
#ifdef DEBUG
			else if(mib->mib_majorID == RES_ID_CBREAK)
				dprintf(("[CBREAK incompatible module found @0x%04x: minor=%u, version=%u]\n"
				 , segm, mib->mib_minorID, mib->mib_version));
#endif
		}
		break;

	case MOD_DUMMY_LOAD:	/* activate dummy module */
		if(state.mode != MOD_ACTIVATED) {
			state.mode = MOD_DUMMY_LOADED;
			setvect(0x23, (isr)cbreak_dummy);
		}
		break;

	case MOD_LOAD:			/* load from resource file */
		if(state.mode != MOD_ATTACHED) {
			FILE *f = va_arg(ap, FILE*);
			res_length_t length = va_arg(ap, res_length_t);
			res_majorid_t major = va_arg(ap, res_majorid_t);
			res_minorid_t minor = va_arg(ap, res_minorid_t);
			res_version_t version = va_arg(ap, res_version_t);

			if(major == RES_ID_CBREAK
			 /* && minor == * */
			 && version == MOD_VERSION
			 && (mib = loadSimpleModule(f, length, major, minor, version
			             , modToken)) != 0)
				closeIntModule(mib, (mod_load_state_t*)&state, modToken
				 , *(void far**)MK_FP(_psp, 0xe));		/* original INT23 */
#ifdef DEBUG
			else if(major == RES_ID_CBREAK)
				dprintf(("[CBREAK incompatible resource found @%lu: minor=%u, version=%u]\n"
				 , length, minor, version));
#endif
		}
		break;

	case MOD_ACTIVATE:
		state.mode = MOD_ACTIVATED;
		break;

	case MOD_ATTACHED:
		if(state.mode != MOD_ATTACHED)
			return 0;
		return MIBtoMCB(state.mib);	

	case MOD_LOADED:
		if(state.mode != MOD_LOADED)
			return 0;
		return MIBtoMCB(state.mib);	

#ifndef NDEBUG
	default:
		fprintf(stderr, "Assertation failed: Invalid action code in CBREAK: %u\n", action);
#endif
	}

	return 1;
}
