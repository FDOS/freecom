/*
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
 * 2000/07/13 ska
 * started
 */

#include <assert.h>
#include <stdio.h>

#include "resource.h"

	/* information about the currently opened resource */
static resource_t res = {
	(long)-1, RES_ID_NONE, 0, 0, RES_COOKIE
};

void startResource(FILE *f, res_majorid_t major, res_minorid_t minor
	, res_version_t version)
{

	assert(f != NULL);
		/* none resource is currently opened */
	assert(res.res_majorID == RES_ID_NONE);

	res.res_length = ftell(f);
	assert(res.res_length != (unsigned long)-1);

	res.res_majorID = major;
	res.res_minorID = minor;
	res.res_version = version;

}

void endResource(FILE *f)
{	unsigned long pos;

	assert(f != NULL);
	assert(res.res_majorID != RES_ID_NONE);

	pos = ftell(f);
	assert(pos != (unsigned long)-1);
	assert(pos >= res.res_length);

	res.res_length = pos - res.res_length;
	fwrite(&res, sizeof(res), 1, f);
}
