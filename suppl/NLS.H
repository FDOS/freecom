/* $Id$
   $Locker$	$Name$	$State$

	DOS NLS implementation -- "use" functions of the NLS info

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.1  1999/04/13 00:11:50  ska
   Initial revision

*/

#ifndef __NLS_H
#define __NLS_H

#include "cntry.h"

struct SupplTime {			/* Universial time/date structure */
			/* This structure is used by all NLS time/date based
				functions and may be initialized partially only */
	int ti_hour, ti_minute, ti_second, ti_hundred;
	int ti_day, ti_month, ti_year;
	int ti_weekday;
};

#ifndef _MICROC_
typedef struct SupplTime nlstime;
#else
#define nlstime struct SupplTime
#endif

/*
 * Fill in the time portions: hour, minute, seconds, hundred seconds
 */
void nlsCurTime(nlstime *t);

/*
 * Fill in the date portions: day, month, year
 */
void nlsCurDate(nlstime *t);


#endif /*#	!defined(__CNTRY_H) */
