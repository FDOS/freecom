/*
 *	Special FreeCOM declaration for the module management
 *
 * 2000/07/12 ska
 * started
 */

#ifndef MODULE_H
#define MODULE_H

#include "resource.h"

#include "algnbyte.h"
/* Module Information Block preceeding each module in memory */
typedef struct {
	res_majorid_t mib_majorID;
	res_minorid_t mib_minorID;
	res_version_t mib_version;
	word mib_linkCount;
	void far*mib_oldint;	/* or reserved */
	byte mib_reserved[4];
} MIB;
typedef MIB __seg *MIBp;
#include "algndflt.h"

#ifndef LINT
#if sizeof(MIB) != 16
#error "The MIB must have 16 bytes in size!"
#endif
#endif

	/* Character used to identify a non-locked, a non-deleted module.
		Characters > NORMAL mean "locked", less mean "deleted".
		The character must not be a valid filename character. */
#define MOD_TOKENID_NORMAL '\6'
#define MOD_TOKENID_DELETE '\2'


typedef enum {
	MOD_INVALID = 0,
	MOD_PRELOAD,
	MOD_LOAD,
	MOD_UNLOAD,
	MOD_ATTACH,
	MOD_DUMMY_LOAD,
	MOD_LOADED,
	MOD_ATTACHED,
	MOD_DUMMY_LOADED,
	MOD_NO,
	MOD_YES,
	MOD_LOCKED,
	MOD_PURGED,
	MOD_ACTIVATE,
	MOD_ACTIVATED,
	MOD_DETACH
} mod_state_t;

#define MOD_LOADSTATE_STRUCT(name)								\
	struct name {												\
		mod_state_t mode;										\
		MIBp mib;

typedef MOD_LOADSTATE_STRUCT(MOD_LOAD_STATE_T)
} mod_load_state_t;

#define MCBtoMIB(segm)	((MIBp )((segm) + 1))
#define MIBtoMCB(mib)	((word)((mib)) - 1)

/*********************************************
	FreeCOM-side interface to the modules.
	They effect all individual modules at once.
 **********************************************/
/* Install dummy modules included within the FreeCOM image.
	They come into effect before the FreeCOM exectuable is known,
	thus the place of the archive the modules reside in is not known
	definitely.
	Though, if the modules are already loaded into memory, they are
	attached right now.  */
void modPreload(void);
/* Attach FreeCOM to all modules.
	Doing so, FreeCOM is automatically detached from any dummy module.
	Returns true, if all modules were attached to successfully. */
int modAttach(void);
/* Return if all modules are loaded into memory and FreeCOM is successfully
	attached to them */
int modAttachedToAll(void);
/* Detach FreeCOM from all modules.
	If a module is no longer linked, it is unloaded from memory. */
void modDetach(void);
/* Load any missing module into memory */
void modLoad(void);


/*********************************************
 	Single module interface
 **********************************************/

/* The "segm" points to the MCB of the module */
/* Allocate a new module of specified length, initialize the MIB, set link count := 1
	This module is locked and length >= 0xffff must result in an error */
word allocModule(res_length_t length, res_majorid_t, res_minorid_t, res_version_t, unsigned alloc_mode);
/* Detach from segmnet:
	decrement link count, unload module if necessary
	the module is unlocked */
void detachModule(word segm);
void unlockModule(word segm);
int lockModule(word segm);

void patchModuleMCB(word segm);
void attachModule(word segm);
MIB far* loadSimpleModule(FILE *f, res_length_t length, res_majorid_t, res_minorid_t, res_version_t, const char name[]);
#define closeModule(a,b,c) closeIntModule((a), (b), (c), 0)
void closeIntModule(MIB far* mib, mod_load_state_t *state, const char name[], void far* oldint);
void attachSimpleModule(word segm, mod_load_state_t *state, const char name[]);

/************************** Modules */
/* ^Break catcher : mod_cbrk.c */
int modCBreak(mod_state_t action, ...);
/* Critical Error Handler : mod_crit.c */
int modCriter(mod_state_t action, ...);
/* Respawner Fail Handler : mod_rspn.c */
int modRspFailed(mod_state_t action, ...);
/* Stub (Persistent Data) : mod_stub.c */
int modStub(mod_state_t action, ...);

#endif
