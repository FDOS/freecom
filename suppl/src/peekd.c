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

ob(ject): peekd
su(bsystem): farmem
ty(pe): A
sh(ort description): Peek a \tok{dword} from a far memory location
lo(ng description): Peeks a \tok{dword} from a far memory location
	or tests, if the word at that location is zero.\par
	If \tok{addr == NULL}, the \tok{dword} is tested only; otherwise it
	is copied into \para{addr}, too.
pr(erequistes): 
va(lue): 0: \tok{(dword far *)MK_FP(segm, ofs) == 0}
	\item else: otherwise
re(lated to): peekdd peekb peekw poked
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

peekd(nM(word) segm, nM(word) ofs, nM(dword *) addr)
{ asm {
	les si, asmName(ofs,6)			; /* source */
	mov bx, asmName(addr,4)			; /* destination address */
	or bx, bx						; /* suppress copying? */
	je peekd1						; /* yes */
	lods Word Ptr es:[si]
	mov [bx], ax
	mov cx, Word Ptr es:[si]
	mov 2[bx], cx
	or ax, cx
	jmp short peekd2
asmLabel(peekd1)					; /* test only */
	lods Word Ptr es:[si]
	or ax, es:[si]
asmLabel(peekd2)
}
}
#endif
