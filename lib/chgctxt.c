/*	$id$
	$Locker$	$Name$	$State$

 	chgCtxt():
 *  Insert/replace/delete a case-sensitive variable into the specified
 *	context
 *
 *  If value == NULL, the variable is deleted.
 *  If value == "", a variable with an empty body is created.
 *  If name == "", a body with an empty name is created.
 *
 *  Return: 0 on success;; !0 on failure

 	chgCtxtRemove():
 		as chgEnv(), but value == "" deletes the variable, too

	This file bases on ENVIRON.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/06/19 20:42:23  skaus
	Update #1

	Revision 1.1  2001/04/12 00:33:52  skaus
	chg: new structure
	chg: If DEBUG enabled, no available commands are displayed on startup
	fix: PTCHSIZE also patches min extra size to force to have this amount
	   of memory available on start
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling
	bugfix: COMMAND.COM A:\
	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
	add: command MEMORY
	bugfix: runExtension(): destroys command[-2]
	add: clean.bat
	add: localized CRITER strings
	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
		via STRINGS.LIB
	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
	add: fixstrs.c: prompts & symbolic keys
	add: fixstrs.c: backslash escape sequences
	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
	chg: splitted code apart into LIB\*.c and CMD\*.c
	bugfix: IF is now using error system & STRINGS to report errors
	add: CALL: /N
	
 */

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int chgCtxt(const Context_Tag tag, const char * const name, const char * const value)
{	word segm, ofs;
	int newlen;

	assert(name);
	ctxtCheckInfoTag(tag);

	segm = ctxtFromTag(tag);
	assert(segm);

	newlen = value? strlen(name) + strlen(value) + 2: 0;
	if((ofs = env_findVar(segm, name)) != (word)-1)
		newlen -= env_varlen(segm, ofs);

	/* Make sure the context has enough room to add the value to */
	if(newlen > 0) {	/* contents size will grow */
		/* aliases may not exceed sizemax */
		if(tag == CTXT_TAG_ALIAS) {
			if(CTXT_INFO(CTXT_TAG_ALIAS, sizemax)
			 - CTXT_INFO(CTXT_TAG_ALIAS, sizecur) < newlen) {
				error_alias_out_of_memory();
				return E_NoMem;
			}
		}

		while(newlen > env_freeCount(ctxtSegm))
			if(ctxtSinglePurge() != E_None) {
				error_context_out_of_memory();
				return E_NoMem;
			}
	}

	/* return values 1 and 3 are OK */
	switch(env_change(segm, name, value)) {
	case 2:			/* variable to delete not found <-> no problem */
	case 1: case 3:   /* var replaced | deleted | inserted ==> OK */
		CTXT_INFO(tag, sizecur) += newlen;
		return 0;
	case 0:       /* Cannot insert */
		dprintf(("chgCtxt(): out-of-mem shouldn't occure here!\n"));
		error_context_out_of_memory();
		return E_NoMem;
	default:
		return E_Syntax;
	}

}
