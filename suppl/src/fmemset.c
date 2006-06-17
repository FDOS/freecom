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

ob(ject): _fmemset
su(bsystem): farmem
ty(pe): 
sh(ort description): Fill a far memory area with a byte
lo(ng description): Files far memory area with the specified byte.
	It must be at least \para{length} bytes long.
pr(erequistes): destination pointer must not be \tok{NULL}
va(lue): none
re(lated to): memset
se(condary subsystems): portable
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

int _fmemset(dseg, dofs , value, length)
asm {
	mov cx, 4[bp]		; the block length to fill
	jcxz return			; nothing to do
	mov di, 8[bp]
	mov ax, 10[bp]
	mov es, ax
	mov al, 6[bp]		; value to put there
	mov ah, al			; double value for word access

	test di, 1		; word align destination pointer
	jz memset1		; already aligned
	stosb			; align by storing one byte
	dec cx
	jz return		; there was only one byte to copy
memset1:
	shr cx, 1		; calculate the amount of words to store
					; Carry holds, whether the amount is even or odd
	rep stosw		; store the memory by words
	jnc memset2		; the amount was even
	stosb			; store the remaining byte
memset2:

return:
}

#else

#ifdef _TC_EARLY_
#include <portable.h>
#include "fmemory.h"

void _fmemset(void far * const dst, int ch, unsigned length)
{	byte far*p;

	if(!length)
		return;

	p = dst;
	do *p++ = ch;
	while(--length);
}

#endif
#endif
