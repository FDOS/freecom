/*	$Id$

	Declarations to access the DOS NLS information

	2001/02/14 ska
	add: nls_makedate / nls_maketime

	$Log$
	Revision 1.2  2001/02/14 23:50:05  skaus
	add: DIR /Y to display 4digit year
	fix: DIR displays "bytes free" at the very end, but if more than one
		argument is specified, it only displays the free bytes of the
		very last argument. The line is now printed after the display
		of each argument.
	bugfix: When the "bytes free" are displayed the used drive letter
		had been deallocated already.
	add: NLS-compatible input of date
	add: NLS-compatible display of date & time (incl DIR)
	fix: parse years 80..199 as century 1900 (by Arkady)
	chg: removed some static variables
	chg: made all global functions & variables static to LOADHIGH.C
	sub: LH.ASM/LOADHIGH.C: duplicate functions (memory API, farmemcmp)
	fix: doc of DATE, TIME, FreeCOM

*/

#ifndef __NLS_H
#define __NLS_H

#include <cntry.h>

extern Country *nlsBuf;		/* internally cached NLS info buffer */

void refreshNLS(void);		/* make sure the nlsBuf is valid */

/* The mode parameter is an OR combination of the following
	defines */
#define NLS_MAKE_SHORT_AMPM	1		/* AM/PM as single letters */


	/* Create a correctly constructed DATE string from the
		given parameters, the result is located in a dynamically
		allocated buffer */
char *nls_makedate(int mode, int year, int month, int day);
char *nls_maketime(int mode, int hour, int minute, int second, int fraction);

#endif
