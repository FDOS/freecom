/*
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
 * 2000/07/13 ska
 * started
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "resource.h"

int enumFileResources(const char *const fnam
	, res_majorid_t id
	, int (*fct)()
	, void * const arg)
{	resource_t res;
	unsigned long pos;
	int rc;
	FILE *f;

	assert(fct);
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
