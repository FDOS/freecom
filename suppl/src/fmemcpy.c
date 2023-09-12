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

ob(ject): _fmemcpy
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Copy a far memory areas onto another
lo: Copies one far memory area onto another one, the destination
	area must be at least \para{length} bytes long.\par
	If both areas overlap themselves, the behaviour is undefined.
he(ader files): 
pr(erequistes): pointers must not be equal to \tok{NULL}
va(lue): none
re(lated to): memcpy _fmemmove
se(condary subsystems): portable
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): Both areas must not overlap each other.
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

void _fmemcpy(unsigned const dseg, unsigned const dofs
 , unsigned const sseg, unsigned const sofs, unsigned length)
{	copy_seg(dseg, dofs, sseg, sofs, length);
}

#else

#if defined(_TC_EARLY_) || defined(__GNUC__)
#include <portable.h>
#include "fmemory.h"

void far *_fmemcpy(void far * const s1, const void far * const s2, unsigned length)
{	byte far*p;
	const byte far*q;

	if(length) {
		p = s1;
		q = s2;
		do *p++ = *q++;
		while(--length);
	}
	return s1;
}

#endif
#endif
