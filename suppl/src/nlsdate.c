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
/* $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): nlsCurDate
su(bsystem): nls
ty(pe): 
sh(ort description): Fill the current date into the NLS time structure
lo(ng description): Fills the current date into the specified NLS structure.
	Only the fields \tok{ti_year}, \tok{ti_month}, \tok{ti_day} and
	\tok{ti_weekday} are modified. The data may range:
 	\item year: 1980..2099
 	\item month: 1-12
 	\item day: 1-31
 	\item weekday: 0..6 (0 == Sunday)
 	\endlist
pr(erequistes): t != NULL
va(lue): none; \tok{*t} structure has been updated
re(lated to): nlsCurTime
se(condary subsystems): portable
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include "nls.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#if defined(_MICROC_) || defined(_TC_EARLY_)
#undef HAVE_DOSDATE
#else
#define HAVE_DOSDATE
#endif

void nlsCurDate(nlstime *t)
{
#ifdef HAVE_DOSDATE
	struct dosdate_t dt;
#else
	USEREGS
#endif

	DBG_ENTER("nlsCurDate", Suppl_nls)
	assert(t);

#ifdef HAVE_DOSDATE
	_dos_getdate(&dt);
	t->ti_year = dt.year;
	t->ti_month = dt.month;
	t->ti_day = dt.day;
	t->ti_weekday = dt.dayofweek;
#else
	_AX = 0x2a00;
	geninterrupt(0x21);
	t->ti_weekday = _AL;
	t->ti_year = _CX;
	t->ti_month = _DH;
	t->ti_day = _DL;
#endif

	DBG_RETURN_VALUES( ("y/m/d=%u/%u/%u, weekday=%u (0==Sun)", t->ti_year, t->ti_month, t->ti_day, t->ti_weekday) )
	DBG_EXIT
}
