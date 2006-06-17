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

	void _fStrUpr(dseg, dofs)

	Upcases the string at dseg:dofs using toUpper().

ob(ject): _fStrUpr
su(bsystem): nls
ty(pe): 
sh(ort description): Upcase the string using the DOS NLS
lo(ng description): Upcases the far memory string using the DOS NLS information.
	The string is overwritten,
pr(erequistes): 
re(lated to): 
se(condary subsystems): farmem
in(itialized by): 
wa(rning): 
bu(gs): 
va: none
fi(le): dstruprf.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

void _fStrUpr(const word dseg, word dofs)
{	int ch;

	DBG_ENTER("_fStrUpr", Suppl_nls)

	while((ch = toUpper(peekb(dseg, dofs))) != NUL)
		pokeb(dseg, dofs++, ch);

	DBG_EXIT
}
