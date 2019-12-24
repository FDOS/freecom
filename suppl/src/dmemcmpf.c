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

ob(ject): _fMemiCmp
su(bsystem): farmem
ty(pe): 
sh(ort description): Compare two far memory areas case-insensitively via DOS NLS
he(ader files): 
lo(ng description): Compares two far memory areas case-insensitively using DOS NLS
pr(erequistes): 
va(lue): <0: first block is less\item =0: both blocks are equal
	\item >0: first block is greater
re(lated to): memcmp, _fMemFCmp
se(condary subsystems): nls
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dmemcmpf.c

*/

#include "initsupl.loc"
#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#ifdef DBCS
# include "mbcs.h"
#endif
#include "dynstr.h"

#include "suppldbg.h"

#ifdef _MICROC_
int _fMemiCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length)
{	int d;

	DBG_ENTER("_fMemiCmp", Suppl_farmem)

	if(!length)
		DBG_RETURN_I( 0)

	if((dseg | dofs) == 0)
		DBG_RETURN_I((sseg | sofs) != 0)
	if((sseg | sofs) == 0)
		DBG_RETURN_I(-1)

	while((d = toUpper(peekb(dseg, dofs++)) - toUpper(peekb(sseg, sofs++)))
	 == 0 && --length);

	DBG_RETURN_I( d)
}

#else

int _fMemiCmp(const byte far * dest, const byte far * src, unsigned length)
{	int d;

	DBG_ENTER("_fMemiCmp", Suppl_farmem)

	if(!length)
		DBG_RETURN_I( 0)

	if(dest == 0)
		DBG_RETURN_I(src != 0)
	if(src == 0)
		DBG_RETURN_I(-1)

#ifdef DBCS
	while(1) {
		unsigned ns = _fMbLen((char far *)src);
		unsigned nd = _fMbLen((char far *)dest);
		if (ns > 1 || nd > 1) {
			d = *dest - *src;
			if (d) break;
			d = dest[1] - src[1];
		}
		else {
			d = toUpper(*dest) - toUpper(*src);
		}
		if (d || length <= ns) break;
		src += ns;
		dest += nd;
		length -= ns;
	}
#else
	while((d = toUpper(*dest++) - toUpper(*src++)) == 0 && --length);
#endif

	DBG_RETURN_I( d)
}
#endif
