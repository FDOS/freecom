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

ob(ject): BLK_byte2para
su(bsystem): portable
sh(ort description): Calculates the number of paragraphes to store \para{num} bytes
va(lue): number of paragraphes 0..0x1000
se(condary subsystems): dosalloc
fi(le): byte2par.c
wa(rning): The argument has a range: 0..0xFFFF

*/

#include "initsupl.loc"


#include <portable.h>

word BLK_byte2para(nM(unsigned) bytes)
#ifdef _MICROC_
asm {
	mov ax, asmName(bytes, 4)
	mov ch, al
	mov cl, 4
	shr ax, cl
	and ch, 0Fh
	jz b2para1		; /* no partially used paragraphe */
	inc ax
b2para1:
}
#else
{ return (bytes >> 4) + ((bytes & 0xF)? 1: 0);	}
#endif
