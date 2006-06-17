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

ob(ject): _fMemFCmp
su(bsystem): nls
ty(pe): H
sh(ort description): case-insensitive memcmp() using DOS NLS for filenames
he(ader files): 
lo(ng description): case-insensitive memcmp() using DOS NLS for filenames;
	the pointers are farmem-pointers
pr(erequistes): 
va(lue): <0: first block is less\item =0: both blocks are equal
	\item >0: first block is greater
re(lated to): 
se(condary subsystems): farmem
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dmemcmp3.c

*/

#include "initsupl.loc"
#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef _MICROC_
int _fMemFCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length)
{	int d;

	DBG_ENTER("_fMemFCmp", Suppl_nls)

	if(!length)
		DBG_RETURN_I( 0)

	if((dseg | dofs) == 0)
		DBG_RETURN_I((sseg | sofs) != 0)
	if((sseg | sofs) == 0)
		DBG_RETURN_I(-1)

	while((d = toFUpper(peekb(dseg, dofs++)) - toFUpper(peekb(sseg, sofs++)))
	 == 0 && --length);

	DBG_RETURN_I( d)
}

#else

int _fMemFCmp(const void far * dest, const void far * src, unsigned length)
{	int d;

	register byte const far *a = dest;
	register byte const far *b = src;

	DBG_ENTER("_fMemFCmp", Suppl_nls)

	if(!length)
		DBG_RETURN_I( 0)

	if(a == 0)
		DBG_RETURN_I(b != 0)
	if(b == 0)
		DBG_RETURN_I(-1)

	while((d = toFUpper(*a++) - toFUpper(*b++)) == 0 && --length);

	DBG_RETURN_I( d)
}
#endif
