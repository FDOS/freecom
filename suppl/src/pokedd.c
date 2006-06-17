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

ob(ject): pokedd
su(bsystem): farmem
ty(pe): A
sh(ort description): Poke a \tok{dword} into far memory
lo(ng description): Pokes a \tok{dword} specified as two \tok{word}
	values into the far memory location \tok{MK_FP(segm, ofs)}
pr(erequistes): 
va(lue): none
re(lated to): poked pokew pokeb peekdd
se(condary subsystems): portable
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

pokedd(/*word*/ segm, /*word*/ ofs, /*word */hi, /*word*/ lo)
asm {
	les di, 8[bp]			; destination offset
	mov ax, 4[bp]
	stosw
	mov ax, 6[bp]
	stosw
}
#endif
