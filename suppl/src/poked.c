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

ob(ject): poked
su(bsystem): farmem
ty(pe): A
sh(ort description): Poke a \tok{dword} at a far memory location
lo(ng description): Pokes the \tok{dword} pointed to by \tok{*addr}
	into the far memory pointed to by \tok{MK_FP(segm, ofs)}.\par
	If \tok{addr == NULL}, a 0 (zero) is poked into the far memory.
pr(erequistes): 
va(lue): none
re(lated to): pokedd pokew pokeb peekd
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

poked(/*word*/ segm, /*word*/ ofs, /*dword **/addr)
asm {
	les di, 6[bp]			; destination
	mov ax, 4[bp]			; source address
	or ax, ax				; special NULL?
	je poked1				; yes, put two 0s
	mov si, ax				; no, copy two words
	movsw
	movsw
	jmp short poked2
poked1:
	stosw
	stosw
poked2:
}
#endif
