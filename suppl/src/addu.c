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

ob(ject): addu
su(bsystem): portable
ty(pe): A
sy(nopsis): int addu(unsigned * const u1, unsigned u2)
sh(ort description): Performs \tok{*\para{u1} += \para{u2}}
	and returns the status of the arethmetic overflow
he(ader files): portable.h
fi(le): addu.c
pr: \para{u1} != NULL
va: !0 if arethemtic overflow occured\item 0: no overflow

*/

#include "initsupl.loc"

#ifndef __WATCOMC__

#include <portable.h>

#ifdef HI_TECH_C

#include "addu.pac"

#else		/* HI_TECH_C */

int addu(nM(unsigned * const) u1, nM(const unsigned) u2)
{
#if defined(__GNUC__)
	int ret;
	asm("add %2, %1; sbb %0, %0": "=r"(ret), "+m"(*u1): "r"(u2));
	return ret;
#elif defined(_TC_EARLY_)
	*u1 += u2;			/* Is translated into exactly what's written
							in #else branch */
	__emit__(0x1b,0xc0);		/* SBB AX, AX */
#else
asm {
		mov ax, asmName(u2, 4)
#ifdef FARDATA
		les bx, asmName(u1, 6)
		add es:[bx], ax
#else	/* FARDATA */
		mov bx, asmName(u1, 6)
		add [bx], ax
#endif	/* FARDATA */
		sbb ax, ax
	}
#endif
#ifdef __TURBOC__
	return _AX;			/* shut up warning */
						/* is removed completely if -O is specified */
#endif
}
#endif		/* HI_TECH_C */

#endif	/* WATCOM */
