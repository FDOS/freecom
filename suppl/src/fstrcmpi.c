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

ob(ject): _fstricmp
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Compare two far strings case-insensitively
lo: Compares two far strings case-insensitively; the function
	\tok{toupper()} of the C standard library is used to ignore the
	case of the characters.
he(ader files): 
pr(erequistes): 
va(lue): 
	\tok{== 0}: both strings are equal
	  \item\tok{> 0}: 1st string > 2nd string (or longer)
	  \item\tok{< 0}: 1st string < 2nd string (or smaller)
re(lated to): _fstrcmp stricmp
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
int _fstricmp(fargDecl(const char, dst), fargDecl(const char, src))
{	return _fmemicmp(fargPass(dst), fargPass(src), _fstrlen1(dst));
}

#if 0		/* optimize for speed than size */

#ifdef _MICROC_
#include <portable.h>

int _fstricmp(unsigned const dseg, unsigned const dofs
 , unsigned const sseg, unsigned const sofs)
{	int d;
	char c;

	while((d = (c = toupper(peekb(dseg, dofs++)))
	 - toupper(peekb(sseg, sofs++))) == 0 && c);

	return d;
}

#else

#ifdef _TC_EARLY_
#include <portable.h>
#include "fmemory.h"

int _fstricmp(const void far * const s1, const void far * const s2)
{	const byte far *p;
	const byte far *q;
	int d;
	byte c;

	if(!length)
		return 0;

	p = s1;
	q = s2;
	while((d = (c = toupper(*p++)) - toupper(*q++)) != 0 && c);
	return d;
}
#endif	/* _TC_EARLY_ */
#endif	/* _MICROC_ */
#endif	/* 0 */
#endif	/* COMPILER */
