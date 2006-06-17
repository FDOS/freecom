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

ob(ject): _fMemUpr
su(bsystem): farmem
ty(pe): 
sh(ort description): Upercases a far memory area using DOS NLS
he(ader files): 
lo(ng description): Upercases a far memory area using DOS NLS
pr(erequistes): 
va(lue): none
re(lated to): _fMemiCmp, MemUpr
se(condary subsystems): nls
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dmemuprf.c

*/

#include "initsupl.loc"
#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

void _fMemUpr(const word dseg, word dofs, unsigned length)
{
	DBG_ENTER("_fMemUpr", Suppl_farmem)

	while(length--) {
		pokeb(dseg, dofs, toUpper(peekb(dseg, dofs)));
		++dofs;
	}

	DBG_EXIT
}
