/* $Id$
 *	Module implementation for CRITER
 *
 * 2000/07/12 ska
 * started
 */

#include <stdarg.h>
#include <limits.h>

#include <dos.h>
#include <mcb.h>

#include "config.h"
#include "debug.h"
#include "error.h"
#include "misc.h"
#include "module.h"

#define NONE (unsigned long)-1

static MOD_LOADSTATE_STRUCT(mod_state_criter_t)
	FILE *f;		/* file the resources reside in, if == NULL --> no file */
		/* Usually the CRITER module has separated code and string portion */
	res_length_t code;		/* CRITER code */
	res_length_t strings;	/* CRITER strings */
	unsigned codeLength, stringsLength;
} state;

#include "criter.ver"			/* module's version */
#include "dmy_crit.inc"			/* dummy criter handler */

typedef void interrupt (*isr)();
static char modToken[] = "CRITER";

int modCriter(mod_state_t action, ...)
{	va_list ap;
	word segm;
	MIB far *mib;

	va_start(ap, action);

	switch(action) {
	case MOD_LOAD:			/* load from resource file */
		if(state.mode != MOD_ATTACHED && state.mode != MOD_LOADED) {
			int i;
			word far *p;
			FILE *f = va_arg(ap, FILE*);
			res_length_t length = va_arg(ap, res_length_t);
			res_majorid_t major = va_arg(ap, res_majorid_t);
			res_minorid_t minor = va_arg(ap, res_minorid_t);
			res_version_t version = va_arg(ap, res_version_t);

			if(major == RES_ID_CRITER
			 && minor == 3		/* both code and data */
			 && version == MOD_VERSION
			 && length < UINT_MAX) {
				if(state.f != f) {
					/* File has been changed */
					state.code = state.strings = NONE;
					state.f = f;
				}
				switch(minor) {
				case 0x01:		/* CRITER code */
					state.code = ftell(f);
					state.codeLength = (unsigned)length;
					if(state.strings == NONE)
						break;
			loadSeparated:		/* load a separated code/data CRITER module */
					if((segm = allocModule(
					 (res_length_t)state.codeLength + state.stringsLength
					 , major, minor, version, 0x82)) == 0) {
						error_fatal_load_module(modToken);
						break;
					}
					if(fseek(f, state.code, SEEK_SET)
					 || farread(MK_FP(segm + 2, 0), state.codeLength, f)
					 	 != state.codeLength
					 || fseek(f, state.strings, SEEK_SET)
					 || farread(MK_FP(segm + 2, state.codeLength)
					     , state.stringsLength, f)
					  != state.stringsLength) {
						dprintf(("[Error reading CRITER module.]\n"));
						detachModule(segm);
						break;
					}
					/* The CRITER strings resource includes offsets relative to
						its beginning, though, the code resource
						requires real offsets. */
					i = *(byte far*)MK_FP(segm + 2, state.codeLength);
					p = (word far*)MK_FP(segm + 2, state.codeLength + 1);
					while(i--)
						*p++ += state.codeLength;

					mib = MCBtoMIB(segm);
			readDone:
					closeIntModule(mib, (mod_load_state_t*)&state, modToken
					 , *(void far**)MK_FP(_psp, 0x12));		/* original INT24 */
					state.mode = MOD_LOADED;
					state.mib = mib;
					break;

				case 0x03:		/* CRITER code and strings merged together */
					if((mib = loadSimpleModule(f, length, major, minor
					 , version, modToken)) != 0) {
						goto readDone;
					}

				case 0x02:		/* CRITER strings */
					state.strings = ftell(f);
					state.stringsLength = (unsigned)length;
					if(state.code != NONE)		/* both found already? */
						goto loadSeparated;
					break;
				}
			}
#ifdef DEBUG
			else if(major == RES_ID_CRITER)
				dprintf(("[CRITER incompatible resource found @%lu: minor=%u, version=%u]\n"
				 , length, minor, version));
#endif
		}
		break;

	case MOD_ATTACH:	/* try to attach to specified already loaded module */
		if(state.mode != MOD_ATTACHED) {
			segm = va_arg(ap, word);
			mib = MCBtoMIB(segm);
			if(mib->mib_majorID == RES_ID_CRITER
			 && mib->mib_minorID == 3			/* code + strings */
			 && mib->mib_version == MOD_VERSION)
				/* may attach to this module */
				attachSimpleModule(segm, (mod_load_state_t*)&state, modToken);
			else {
#ifdef DEBUG
				if(mib->mib_majorID == RES_ID_CRITER)
					dprintf(("[CRITER incompatible module found @0x%04x: minor=%u, version=%u]\n"
					 , segm, mib->mib_minorID, mib->mib_version));
#endif
				return 0;
			}
		}
		break;

	case MOD_PRELOAD:	/* activate dummy module */
		if(state.mode != MOD_ACTIVATED)
			setvect(0x24, (isr)criter_dummy);
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
		fprintf(stderr, "Assertation failed: Invalid action code in CRITER: %u\n", action);
#endif
	}

	return 1;
}
