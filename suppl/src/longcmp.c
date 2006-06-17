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

ob(ject): longcmp
su(bsystem): longmath
ty(pe): A
sh(ort description): Compare two 32bit numbers
lo(ng description): 
pr(erequistes): 
va(lue): -1: \tok{d1 < d2}
	\item 0: \tok{d1 == d2}
	\item +1: \tok{d1 > d2}
re(lated to): longtst longcmp1
se(condary subsystems): portable
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): non-Micro-C only

*/

#include "initsupl.loc"

#ifndef _MICROC_

#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int longcmp(long d1, long d2)
{
	return d1 < d2
		? -1
		: d1 == d2?	0: 1;
}

#endif
