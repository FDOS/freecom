/*	$Id$

 *  RES_W.C - Write resource
 *
 *	Contains all functions to encapsule to "write" resources.
 *
 *	startResource(FILE *f, res_majorid_t , res_minorid_t)
 *		Starts a new resource and issues the necessary prologue.
 *		All the data should be dumped immediately now.
 *
 *	endResource(FILE *f)
 *		ends the resource and write the necessary epilogue. The
 *		should be called when all data has been dumped; when the function
 *		returns the file is positioned immediately behind the last byte
 *		allocated by the resource.
 *
 *
 *	Both functions must be called in sequence, that means first
 *	startResource(), then endResource(), but no intermixing or
 *	end() before start().
 *

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

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

#include <assert.h>
#include <stdio.h>

#include "../include/resource.h"

	/* information about the currently opened resource */
static resource_t res = {
	(long)-1, RES_ID_NONE, 0, RES_COOKIE
};

void startResource(FILE *f, res_majorid_t major, res_minorid_t minor)
{

	assert(f != 0);
		/* none resource is currently opened */
	assert(res.res_majorID == RES_ID_NONE);

	res.res_length = ftell(f);
	assert(res.res_length != (unsigned long)-1);

	res.res_majorID = major;
	res.res_minorID = minor;

}

void endResource(FILE *f)
{	unsigned long pos;

	assert(f != 0);
	assert(res.res_majorID != RES_ID_NONE);

	pos = ftell(f);
	assert(pos != (unsigned long)-1);
	assert(pos >= res.res_length);

	res.res_length = pos - res.res_length;
	fwrite(&res, sizeof(res), 1, f);
}
