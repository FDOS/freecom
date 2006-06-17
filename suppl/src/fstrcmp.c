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

ob(ject): _fstrcmp
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Compare two far strings
he(ader files): 
lo(ng description): 
pr(erequistes): 
va(lue): 
	\tok{== 0}: both strings are equal
	  \item\tok{> 0}: 1st string > 2nd string (or longer)
	  \item\tok{< 0}: 1st string < 2nd string (or smaller)
re(lated to): strcmp _fstricmp
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

#include "fmemory.loc"

#ifdef COMPILE
int _fstrcmp(fargDecl(const char, dst), fargDecl(const char, src))
{	return _fmemcmp(fargPass(dst), fargPass(src), _fstrlen1(dst));
}

#if 0		/* optimize for speed than size */
#ifdef _MICROC_
#include <portable.h>

int _fstrcmp(unsigned const dseg, unsigned const dofs
 , unsigned const sseg, unsigned const sofs)
{	int d;
	char c;

	while((d = (c = peekb(dseg, dofs++)) - peekb(sseg, sofs++)) == 0 && c);

	return d;
}

#else

#ifdef _TC_EARLY_
#include <portable.h>
#include "fmemory.h"

int _fstrcmp(const void far * const s1, const void far * const s2)
{	const byte far *p;
	const byte far *q;
	int d;
	byte c;

	if(!length)
		return 0;

	p = s1;
	q = s2;
	while((d = (c = *p++) - *q++) != 0 && c);
	return d;
}
#endif
#endif

#endif
#endif
