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

ob(ject): nlsCurTime
su(bsystem): nls
ty(pe): 
sh(ort description): Fill the current time into the NLS structure
lo(ng description): Fills the current time into the specified NLS
	structure. This function only modifies the fields \tok{ti_hour},
	\tok{ti_minute}, \tok{ti_second} and \tok{ti_hundred}. The
	data may range in:
 	\item hour: 0..23
 	\item minute: 0..59
 	\item second: 0..59
 	\item hundred: 0..99
 	\endlist
pr(erequistes): t != NULL
va(lue): none; \tok{*t} structure has been updated
re(lated to): nlsCurDate
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

#if defined(_MICROC_) || defined(_TC_EARLY_) || defined(__GNUC__)
#undef HAVE_DOSTIME
#else
#define HAVE_DOSTIME
#endif

void nlsCurTime(nlstime *t)
{
#ifdef HAVE_DOSTIME
	struct dostime_t dt;
#else
	USEREGS
#endif

	DBG_ENTER("nlsCurTime", Suppl_nls)
	assert(t);

#ifdef HAVE_DOSTIME
	_dos_gettime(&dt);
	t->ti_hour = dt.hour;
	t->ti_minute = dt.minute;
	t->ti_second = dt.second;
	t->ti_hundred = dt.hsecond;
#else
	_AX = 0x2c00;
	geninterrupt(0x21);
	t->ti_hour = _CH;
	t->ti_minute = _CL;
	t->ti_second = _DH;
	t->ti_hundred = _DL;
#endif

	DBG_RETURN_VALUES( ("h:m:s.h=%u:%u:%u.%u", t->ti_hour, t->ti_minute, t->ti_second, t->ti_hundred) )
	DBG_EXIT
}
