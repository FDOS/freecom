/*
 *	Special FreeCOM defuinitions for the module management
 *
 *	The interface does only depends on the Stub, because all the
 *	other shared modules are referenced through the stub only.
 *	The only exception is preload (Action A2), because the preloaded
 *	dummy modules provide reduced services especially at the there is
 *	no "full" module loaded.
 *	
 * 2000/07/12 ska
 * started
 */

#include <assert.h>
#include <dos.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <mcb.h>
#include <suppl.h>

#include "config.h"
#include "debug.h"
#include "error.h"
#include "misc.h"
#include "module.h"
#include "res.h"

char const modToken[] = "FREECOM";
#ifndef LINT
#if sizeof(modToken) != 8
#error "modToken must be 7 bytes in size (+ NUL)"
#endif
#endif

/* Test if the memory @segm contains a valid
	module (MCB) */
mod_state_t modType(word segm)
{	int mode;

	if(_fmemcmp(MK_FP(segm, MCB_OFF_NAME), modToken, sizeof(modToken)-1) != 0)
		return MOD_NO;			/* no module */

	if((mode = peekb(segm, MCB_OFF_NAME + 7)) < 5)
		return MOD_PURGED;			/* purged module */

	if(mode == 5)
		return MOD_YES;

	return MOD_LOCKED;
}


void attachModule(word segm)
{	MIB far*mib;

	mib = MCBtoMIB(segm);
	if(++(mib->mib_linkCount) == 0) {
		/* overflow */
		mib->mib_linkCount = -1;
	}
}

void detachModule(word segm)
{	MIB far*mib;

	mib = MCBtoMIB(segm);
	if(mib->mib_linkCount != (unsigned)-1) {	/* permanently locked */
		if(--mib->mib_linkCount == 0) {	/* purge the module from memory */
			struct MCB far*mcb;

			mcb = MK_FP(segm, 0);
			mcb->mcb_name[7] = MOD_TOKENID_DELETE;
			mcb->mcb_ownerPSP = _psp;	/* so we may deallocate the block */
			freeBlk(segm + 1);
			return;
		}
	}

	unlockModule(segm);
}

MIB far* loadSimpleModule(FILE *f
	, unsigned long length
	, res_majorid_t major
	, res_minorid_t minor
	, res_version_t version
	, const char name[])
{	word segm;

	if((segm = allocModule(length, major, minor, version, 0x82)) == 0) {
		error_fatal_load_module(name);
		return 0;
	}
	if(farread(MK_FP(segm + 2, 0), (unsigned)length, f) != (unsigned)length) {
		dprintf(("[Error reading %s module.]\n", name));
		detachModule(segm);
		return 0;
	}

	return MK_FP(segm + 1, 0);
}

void closeIntModule(MIB far* mib, mod_load_state_t *state, const char name[], void far* oldint)
{
	mib->mib_oldint = oldint;
	unlockModule(MIBtoMCB(state->mib = mib));
	state->mode = MOD_ATTACHED;
	dprintf(("[%s loaded and attached to 0x%04x]\n", name
	 , MIBtoMCB(state->mib)));
}

void attachSimpleModule(word segm, mod_load_state_t *state, const char name[])
{
	attachModule(segm);
	state->mib = MCBtoMIB(segm);
	state->mode = MOD_ATTACHED;
	dprintf(("[%s attaching to segment 0x%04x.]\n", name, segm));
}

int lockModule(word segm)
{	struct MCB far* mcb;

	if(modType(segm) == MOD_YES) {
		mcb = MK_FP(segm, 0);
		/* May fail, if another process checks the module
			right between the 'if' and before the decrement */
		if(++mcb->mcb_name[7] == MOD_TOKENID_NORMAL + 1)
			return 1;			/* successfully locked */
		/* failed */
		--mcb->mcb_name[7];
	}
	return 0;
}

void unlockModule(word segm)
{	struct MCB far* mcb;

	mcb = MK_FP(segm, 0);
	--mcb->mcb_name[7];
}

void patchModuleMCB(word segm)
{	struct MCB far*mcb;

	assert(segm > 0x40);
	mcb = MK_FP(segm);
	_fmemcpy(mcb->mcb_name, (void far*)modToken, 7);
	mcb->mcb_name[7] = MOD_TOKENID_NORMAL + 1;		/* locked */
	mcb->mcb_ownerPSP = 8;		/* Mark this block of memory as "system" */
}

/* Allocate a new module of specified length,
	initialize the MIB, set link count := 1, lock module.
	If length >= 0xffff must result in an error.
	Display any error messages. */
word allocModule(res_length_t length
	, res_majorid_t majorID
	, res_minorid_t minorID
	, res_version_t version
	, unsigned alloc_mode)
{	word segm;
	MIB far*mib;

	/* Allocate a reasonable block of memory */
	if(length >= UINT_MAX) {
		dprintf(("Module too large: %ld, major = %u, minor = %u, version = %u\n", length, majorID, minorID, version));
		return 0;
	}

	assert(sizeof(MIB) == 16);
	/* Transform the number of bytes into paragraphes
		one additional paragraphe for the MIB */
	if((segm = allocBlk(((unsigned)length + 15) / 16 + 1, alloc_mode)) == 0) {
		dprintf(("Out of memory: %ld, major = %u, minor = %u, version = %u\n", length, majorID, minorID, version));
		return 0;
	}

	/* Initialize MIB */
	mib = MK_FP(segm, 0);
	_fmemset(mib, 0, sizeof(MIB));
	mib->mib_majorID = majorID;
	mib->mib_minorID = minorID;
	mib->mib_version = version;
	mib->mib_linkCount = 1;

	/* Update MCB */
	patchModuleMCB(MIBtoMCB(mib));

	return MIBtoMCB(mib);
}

/*****************************
	Functions effecting all modules at once.
		********************************/

void modPreload(void)
{
	modCriter(MOD_PRELOAD);
	modCBreak(MOD_PRELOAD);
	modStub(MOD_PRELOAD);
}

int modAttach(void)
{
	modStub(MOD_ATTACH);
	return modStub(MOD_ATTACHED) != 0;
}

void modLoad(void)
{
	modStub(MOD_LOAD);
}

void modDetach(void)
{
	modStub(MOD_DETACH);
}
