/*	$id$
	$Locker$	$Name$	$State$

 *  RES_R.C - Read resource
 *
 *	Contains all functions to encapsule to "read" resources.
 *
 *	int enumFileResources(fnam, res_majorid_t, int (*fct)(), void * arg)
 *		Searches for all resources of the specified major ID in the
 *		specified file.
 *		The callback function is called like this:
 *			int fct(res_majorid_t, res_minorid_t, length, FILE*, arg)
 *		The "length" parameter specifies the length of the data
 *		block (unsigned long), the file is the resource file opened
 *		in "r+b" modus <<be careful!!>>, "arg" is the pointer passed to
 *		enumREsources().
 *		The callback function may reposition the file, though, the file
 *		itself must not be closed.
 *
 *		If the callback function returns 0 (zero), the loop continues,
 *		otherwise enumResource() is terminated and this value is returned.
 *
 *	Return:
 *		0: Nothing found or callback function always returned "0" itself
 *		else: return value of callback function
 *		-1: file open error
 *

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
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
#include <string.h>
#include <stdlib.h>

#include "../include/resource.h"

int enumFileResources(const char *const fnam
	, res_majorid_t id
	, int (*fct)()
	, void * const arg)
{	resource_t res;
	unsigned long pos;
	int rc;
	FILE *f;

	assert(fct);
	assert(fnam);

	rc = 0;
	if((f = fopen(fnam, "r+b")) == 0) {
	 	rc = -1;

		/* resource file could be opened --> seek to its end to
			get the first resource */
	} else if(fseek(f, -(long)sizeof(res), SEEK_END) == 0) do {
		pos = ftell(f);
		assert(pos != (unsigned long)-1l);
	} while(fread(&res, sizeof(res), 1, f) == 1
		 && memcmp(res.res_cookie, RES_COOKIE, sizeof(res.res_cookie)) == 0
		 && pos >= res.res_length
			/* resources handled:
				1) cookie mismatch <-> magic number is invalid
				2) length is larger than avilable bytes
			*/
			/* position of data block */
		 && fseek(f, pos -= res.res_length, SEEK_SET) == 0
		 && ((res.res_majorID != id && id != RES_ID_ANY)
			|| (rc = fct(res.res_majorID
				 , res.res_minorID
				 , res.res_length
				 , f
				 , arg)) == 0
		    )
		 && pos >= sizeof(res)
		 && fseek(f, pos - sizeof(res), SEEK_SET) == 0
	  );

	fclose(f);
	return rc;
}
