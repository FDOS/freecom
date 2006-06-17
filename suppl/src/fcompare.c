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

ob(ject): _fcompare
su(bsystem): farmem
ty(pe): L
sy(nopsis): 
sh(ort description): Compare two normalized far pointers
he(ader files): 
lo(ng description): Compares two normalized far pointers. Normalized
	far pointers have an offset less than 16.
pr(erequistes): 
va(lue): 
	\item == 0: both pointers are equal
		\item	  > 0: 1st pointer > 2nd pointer
		\item	  < 0: 1st pointer < 2nd pointer
re(lated to): _fnormalize
se(condary subsystems): portable
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"
#ifdef _MICROC_

#include <portable.h>

int _fcompare(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs)
{	int seg, ofs;

	seg = dseg - sseg;
	ofs = dofs - sofs;

	if(seg < 0 || (seg == 0 && ofs < 0))
		return -1;
	return !!(seg | ofs);
}

#endif
