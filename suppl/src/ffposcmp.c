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

ob(ject): Fposcmp
su(bsystem): supplio
ty(pe): AU
sh(ort description): Compare two \tok{(fpos_t)} stream positions
pr(erequistes): pos1 != NULL && pos2 != NULL
va(lue): 
		<0: if pos1 < pos2
	\item	=0: if pos1 = pos2
	\item	>0: if pos1 > pos2
re(lated to): Fpos2dword Fposcpy
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <stdio.h>
#include <portable.h>
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#undef Fposcmp
int Fposcmp(const fpos_t * const pos1, const fpos_t * const pos2)
{
#ifndef NDEBUG
	/** Warning: Condition is always false **/
	if(sizeof(fpos_t) != sizeof(long)) {
		/** Warning: Unreachable code **/
#ifndef _MICROC_
		fputs("Your compiler has an incompatible (fpos_t) type\n", stderr);
		abort();
#else
		abort("Your compiler has an incompatible (fpos_t) type");
#endif
	}
#endif

	assert(pos1);
	assert(pos2);

	return longcmp(nM(*(unsigned long*))pos1, nM(*(unsigned long*))pos2);
}
