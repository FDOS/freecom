/*
 * NLS.C -- Support for DOS NLS information
 *
 *  By default COMMAND assumes that the NLS information do change.
 *  refreshNLS() will make sure that nlsBuf is updated and on
 *  occasion when the information could have been changed, nlsBuf
 *  is invalidated in order to be re-read by refreshNLS().
 *
 * 1999/04/27 ska
 *  started
 * add: nlsBuf, refreshNLS
 *
 * 2001/02/14 ska
 * add: nls_makedate() & nls_maketime()
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "nls.h"
#include "command.h"
#include "strings.h"
#include "cmdline.h"

#ifdef FEATURE_NLS

Country *nlsBuf = 0;

void refreshNLS(void)
{ if(!nlsBuf)
    nlsBuf = nlsNewInfo();
}

#endif


#pragma argsused
char *nls_makedate(int mode, int year, int month, int day)
{	char buf[4 + 3 + sizeof(int) * 8 * 3];

#ifdef FEATURE_NLS
	refreshNLS();

	switch(nlsBuf->datefmt) {
	case 0:			/* mm/dd/yy */
		sprintf(buf, "%.2u%s%.2u%s%02u", month, nlsBuf->dateSep, day
		 , nlsBuf->dateSep, year);
		break;
	case 1:			/* dd/mm/yy */
		sprintf(buf, "%02u%s%02u%s%02u", day, nlsBuf->dateSep, month
		 , nlsBuf->dateSep, year);
		break;
	case 2:			/* yy/mm/dd */
		sprintf(buf, "%02u%s%02u%s%02u", year, nlsBuf->dateSep, month
		 , nlsBuf->dateSep, day);
		 
		break;
	}
#else
	sprintf(buf, "%.2u-%.2u-%02u", month, day, year);
#endif

	return strdup(buf);
}


char *nls_maketime(int mode, int hour, int minute, int second, int fraction)
{	char buf[4 + 4 + sizeof(int) * 8 * 4 + 6];
	char *p, *q;
	int i, pm;

#ifdef FEATURE_NLS
	refreshNLS();
#define fmt nlsBuf->timefmt
#define sep nlsBuf->timeSep
#define dsep nlsBuf->decimalSep
#else
#define fmt 0
#define sep ":"
#define dsep "."
#endif

	/** Warning: condition always true -- if !NLS **/
	if(fmt == 0) {		/* 12hour display */

		if((pm = hour >= 12) != 0) {
			hour -= 12;
		}
		if(hour == 0)
			hour = 12;

		i = sprintf(p = buf, "%2u%s%02u", hour, sep, minute);
	} else {
		/** Warning: unreachable code -- if !defined(NLS) **/
		i = sprintf(p = buf, "%02u%s%02u", hour, sep, minute);
	}

	if(i == EOF) return 0;
	if(second >= 0) {
		i = sprintf(p += i, "%s%02u", sep, second);
		if(i == EOF) return 0;
		if(fraction) {
			i = sprintf(p += i, "%s%u", dsep, fraction);
			if(i == EOF) return 0;
		}
	}

	/** Warning: conditional always true -- if !NLS **/
	if(fmt == 0) {
		q = fetchString(pm? TEXT_STRING_PM: TEXT_STRING_AM);
		if(!q)		return 0;
		if(mode & NLS_MAKE_SHORT_AMPM) {
			*(p += i) = *ltrim(q);
			p[1] = 0;
		} else
			strcpy(p + i, q);
	}

	return strdup(buf);
}

