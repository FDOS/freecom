/*	$Id$

	Criter repeat check interface

	$Log$
	Revision 1.1  2004/09/13 18:59:39  skaus
	add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}

 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <stdlib.h>

#include <dynstr.h>

#include "../include/misc.h"
#include "../err_fcts.h"

static word far *repCheckAddr = 0;
static int drives = 0;
#define chkByteArea ((byte far *)(&repCheckAddr[1]))

void registerCriterRepeatCheckAddr(word far *p)
{
	if(p) {
		if(*p == (unsigned)-1) {
			byte far *q = (byte far *)(&p[1]);

			while(*q == (byte)-1) {
				++drives;
				++q;
			}
			dprintf(("[CRITER: Repeat check addr %Fp, drives = %u]\n", p, drives));
			if(drives)
				repCheckAddr = p;
		} else {
			dprintf(("[CRITER: Invalid repeat check addr given: %Fp]\n", p));
		}
	}
}

void critEnableRepeatCheck(void)
{
	if(drives) {
		assert(repCheckAddr);
		_fmemset(chkByteArea, -1, drives);
		*repCheckAddr = 0;
		dprintf(("{CRITER: Enabled repeat check]\n"));
	}
}
unsigned critDisableRepeatCheck(void)
{	if(drives) {
		unsigned count = *repCheckAddr;

		assert(repCheckAddr);
		*repCheckAddr = (unsigned)-1;
		dprintf(("{CRITER: Disabled repeat check: count = %u]\n", count));
		return count;
	}

	return 0;
}
char *critDriveReport(void)
{	if(drives) {
		char *rep = malloc(drives + 1);

		if(rep) {
			int i;
			assert(repCheckAddr);
			assert(drives > 0 && drives < 256);

			*rep = drives;
			_fmemcpy(&rep[1], chkByteArea, drives);
			for(i = 1; i <= drives; ++i)
				 ++rep[i];

			return rep;
		} else
			error_out_of_memory();
	}

	return 0;
}
