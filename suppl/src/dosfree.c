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

ob(ject): DOSfree
su(bsystem): farmem
ty(pe): 
sh(ort description): De-allocate a block of memory using the DOS API
he(ader files): 
lo(ng description): De-allocates a block of memory directly via the DOS API.
	This block must be allocated by \tok{DOSalloc()} or any other function using
	the DOS API to deallocate (free) a block of memory.
pr(erequistes): 
va(lue): 0: on success\item else: DOS error code
re(lated to): DOSalloc DOSresize
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dosfree.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "suppl.h"

#include "suppldbg.h"

int DOSfree(nM(const word) segm)
#ifdef _MICROC_
#ifndef SUPPL_LOG_FUNCTIONS
asm {
	mov ax, asmName(segm, 4)
	or ax, ax				; /* no segment to free */
	jz DOSfree1	
	mov es, ax
	mov ah, 49h
	int 21h
DOSfree1:
}
#else
{	IREGS r;

	DBG_ENTER("DOSfree", Suppl_farmem)
	DBG_ARGUMENTS( ("segm=%04x", segm) )

	r.r_ax = 0x4900;
	r.r_es = segm;
	DBG_RETURN_BI(invokeDOS(aS(r)));
}
#endif
#else
{	USEREGS

	DBG_ENTER("DOSfree", Suppl_farmem)
	DBG_ARGUMENTS( ("segm=%04x", segm) )

	_ES = segm;
	_AH = 0x49;		/* Free Memory Block */
	geninterrupt(0x21);
	DBG_RETURN_BI( _AX)
}
#endif
