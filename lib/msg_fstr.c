/*	$id$
	$Locker$	$Name$	$State$

	read the string from the resource and return a dummy entry
	on failure

	This file bases on MESSAGES.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/07/11 21:46:26  skaus
	Update #8

	Revision 1.1  2001/04/12 00:33:53  skaus
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
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include <fmemory.h>

#include "../include/misc.h"
#include "../strings.h"
#include "../include/strings.typ"

char *getString(const unsigned id)
{
	char *thisstr;
	unsigned segm;
    string_index_t far *idx;

	  /* Issue the default message, if:
  		+ strings could not be loaded into memory,
  		+ requested string ID is not included with resource
  			(too high, or not defined).
	*/
	if((segm = msgSegment()) == 0
	 || id >= strCnt
	 || (idx = MK_FP(msgSegm, id * sizeof(*idx)))->size == 0)
		return defaultMessage(id);

	/* pointer to id's control data */
	if((thisstr = malloc(idx->size)) == 0)
		return defaultMessage(MSG_ERR_ID_OUTOFMEMORY);

	_fmemcpy((char far*)thisstr, MK_FP(segm, idx->index), idx->size);

	return thisstr;
}
