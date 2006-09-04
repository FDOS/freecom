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

ob(ject): peekdd
su(bsystem): farmem
ty(pe): 
sh(ort description): Peek and test a \tok{dword} from far memory
lo(ng description): Tests a \tok{dword} located at a
	far memory location, it it is zero; and, if permitted, peeks the
	\tok{dword} into the \tok{word} variables pointed to by \tok{*hi} and
	\tok{*lo}.\par
	If \tok{hi == NULL} or \tok{lo == NULL}, the particular \tok{word}
	is not copied from far memory, but is included within the test
	nonetheless.
pr(erequistes): 
va(lue): 
		0: both hi-word and lo-word are zero
			\item 1: hi-word is non-zero
			\item 2: lo-word is non-zero
			\item 3: both are non-zero
re(lated to): peekd peekw peekb pokedd
se(condary subsystems): portable
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

peekdd(word segm, word ofs, word *hi, word *lo)
{ 	word h, l, res;

    res = (h = peekw (segm, ofs + 2)) != 0;
	if((l = peekw(segm, ofs)) != 0)
		res |= 2;
	if(hi) *hi = h;
	if(lo) *lo = l;
	return res;
}
