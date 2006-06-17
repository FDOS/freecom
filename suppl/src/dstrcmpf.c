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

	int _fStriCmp(dseg, dofs, sseg, sofs)

	Compares the strings at sseg:sofs and dseg:dofs, case-insensitive using
	toUpper().

	Returns: == 0: both strings are equal
			  > 0: 1st string > 2nd string (or longer)
			  < 0: 1st string < 2nd string (or smaller)

	Target compilers: Micro-C v3.13, v3.14; Borland C v2, v3.1, v4.52

ob(ject): _fStriCmp
su(bsystem): dynstr
ty(pe): 
sh(ort description): Compare two far strings case-insensitively
lo(ng description): Compares two far memory strings case-insensitively using the
	DOS NLS.
pr(erequistes): 
re(lated to): 
se(condary subsystems): nls, farmem
in(itialized by): 
wa(rning): 
bu(gs): 
value: <0: first string is less\item =0: both are equal\item >0: first string is greater
fi(le): dstrcmpf.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

int _fStriCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs)
{	int d, c;

	DBG_ENTER("_fStriCmp", Suppl_dynstr)

	while((d = (c = toUpper(peekb(dseg, dofs++)))
	 - toUpper(peekb(sseg, sofs++))) == 0 && c);

	DBG_RETURN_I( d)
}
