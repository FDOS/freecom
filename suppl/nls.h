/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id$
   $Locker$	$Name$	$State$

	DOS NLS implementation -- "use" functions of the NLS info

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
