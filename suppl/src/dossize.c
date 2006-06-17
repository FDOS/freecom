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

ob(ject): DOSresize
su(bsystem): farmem
ty(pe): 
sh(ort description): Resize a block of memory using the DOS API
he(ader files): 
lo(ng description): Resizes a block of memory directly via the DOS API.
	This block must be allocated by \tok{DOSalloc()} or any other function using
	the DOS API.
pr(erequistes): 
va(lue): 0: on success\item else: on failure
re(lated to): DOSalloc, DOSfree
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dossize.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "suppl.h"
#include "mcb.h"

#include "suppldbg.h"

int DOSresize(word segm, word newLen)
{	
	USEREGS

	DBG_ENTER("DOSresize", Suppl_farmem)
	DBG_ARGUMENTS( ("segm=0x%04x, len=%u", segm, newLen) )

	if(!segm)
		DBG_RETURN_I( 1)		/* no segment */

#ifdef _MICROC_	/* Micro-C's int86() does not support ES */
	segm;			/* mov ax, segm */
	asm "mov es, ax";
	newLen;			/* mov ax, newLen */
	asm {
		mov bx, ax
		mov ah, 4ah
		int 21h
		sbb ax, ax
	}
#ifdef SUPPL_LOG_FUNCTIONS
	DBG_RETURN_BI(nargs())
#endif
	/* implizitly returning AX */
#else
	_ES = segm;		/* block to resize */
	_BX = newLen;	/* new size in paragraphs */
	_AH = 0x4a;		/* DOS call Modify Allocated Memory */
	geninterrupt(0x21);
	DBG_RETURN_BI( _CFLAG)		/* return the Carry flag */
#endif
}

#ifdef _MICROC_
word resizeBlk(const word segm, const unsigned length) 
{	return DOSresize(segm, BLK_byte2para(length));
}
#endif
