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

ob(ject): _fmemcmp
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Compare two far memory areas
he(ader files): 
pr(erequistes): pointers must not be equal to \tok{NULL}
va(lue): 
	 \tok{== 0}: both areas are equal
	\item \tok{> 0}: 1st area > 2nd area
	  \item \tok{< 0}: 1st area < 2nd area
re(lated to): memcmp _fmemicmp
se(condary subsystems): portable
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C and Pacific HiTech C

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

int _fmemcmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length)
{	int d;

	if(!length)
		return 0;

	while((d = peekb(dseg, dofs++) - peekb(sseg, sofs++)) == 0 && --length);

	return d;
}

#else

#if defined(_PAC_NOCLIB_) || defined(_TC_EARLY_) || defined(__GNUC__)
#include <portable.h>
#include "fmemory.h"

int _fmemcmp(const void far * const s1, const void far * const s2
	, unsigned length)
{	const byte far *p;
	const byte far *q;
	int d;

	if(!length)
		return 0;

	p = s1;
	q = s2;
	while((d = *p++ - *q++) == 0 && --length);
	return d;
}
#endif
#endif
