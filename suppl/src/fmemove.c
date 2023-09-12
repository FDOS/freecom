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

ob(ject): _fmemmove
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Copy a far memory areas onto another
lo: Copies one far memory area onto another one, the destination
	area must be at least \para{length} bytes long. Both areas may
	overlap themselves.
he(ader files): 
pr(erequistes): pointers must not be equal to \tok{NULL}
va(lue): none
re(lated to): memmove _fmemcpy
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
#include "fmemory.h"

void _fmemmove(unsigned dseg, unsigned dofs
 , unsigned sseg, unsigned sofs, unsigned length)
{	unsigned seg, ofs;

	if(!length)
		return;

	/* 1st: normalize pointers for compare */
	_fnormalize(dseg, dofs);
	_fnormalize(sseg, sofs);

	if(sseg == dseg && sofs == dofs)
		return;

	seg = sseg;				/* end of source area */
	ofs = sofs + length;
	_fnormalize(seg, ofs);

	/* 2nd: check, if the areas overlap and if this matters at all */
	/* Bad results will occur, if sseg:sofs < dseg:dofs < seg:ofs */
	if(_fcompare(sseg, sofs, dseg, dofs) < 0
	 && _fcompare(dseg, dofs, seg, ofs) <= 0) {	/* perform manually */
	 	dofs += length;		/* both point behind the areas now */
	 	sofs += length;
		do pokeb(dseg, --dofs, peekb(sseg, --sofs));
		while(--length);
	}
	else _fmemcpy(dseg, dofs, sseg, sofs, length);
}

#else
#if defined(_PAC_NOCLIB_) || defined(_TC_EARLY_) || defined(__GNUC__)
#include <portable.h>
#include "fmemory.h"

void far *_fmemmove(void far * const s1, const void far * const s2
	, unsigned length)
{	byte far *p;
	byte far *q;
	byte far *h;

	if(!length)
		return s1;

	p = _fnormalize(s1);
	q = _fnormalize((void far*)s2);
	h = _fnormalize(q + length);

	/* Cases:
		p == q --> nothing to do

		no overlapping areas --> call _fmemcpy()

		p < q && overlapping -->
			   ..ABCDE
			   1234
			If the string ABCDE is copied from the beginning one-by-one
			over 1234, the result is OK.
				--> call _fmemcpy()

		p > q && overlapping -->
				ABCDE
				..1234
			If now one starts from the beginning of ABCDE, the 'A' will
			overwrite the position of '1', which is also 'E' ---
			but 'E' would be copied later another time.
				--> copy backwardly
	*/
	if(p == q)
		return s1;

	/*
	 * without the typecasts TC++1 ignores the segment portions completely
	 */
#ifdef __GNUC__
	if(p > q && p <= h) {
#else
	if((char huge*)p > (char huge*)q && (char huge*)p <= (char huge*)h) {
#endif
		/* overlapping areas */
		p += length;
		q += length;
		do *--p = *--q;
		while(--length);
	}
	else {
		_fmemcpy(s1, s2, length);
	}
	return s1;
}
#endif
#endif
