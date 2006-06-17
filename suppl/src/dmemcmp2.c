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

ob(ject): MemFCmp
su(bsystem): nls
ty(pe): H
sh(ort description): case-insensitive memcmp() using DOS NLS for filenames
he(ader files): 
lo(ng description): case-insensitive memcmp() using DOS NLS for filenames
pr(erequistes): 
va(lue): <0: first block is less\item =0: both blocks are equal
	\item >0: first block is greater
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dmemcmp2.c

*/

#include "initsupl.loc"
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

int MemFCmp(const void *s, const void *p, unsigned len)
{	int result;

#ifndef _MICROC_
	register byte const *a = s, *b = p;
#else
#define a ((byte*)s)
#define b ((byte*)p)
#endif

	DBG_ENTER("MemFCmp", Suppl_nls)
	DBG_ARGUMENTS( ("s1=\"%s\", s2=\"%s\", len=%u", a, b, len) )

	if(!len)
		DBG_RETURN_I(0)

	if(b == 0)
		DBG_RETURN_I(a != 0)
	if(a == 0)
		DBG_RETURN_I(-1)

	while(!(result = toFUpper(*a++) - toFUpper(*b++)) && --len);

	DBG_RETURN_I( result)
}
