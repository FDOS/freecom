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


ob(ject): _fnormalize
su(bsystem): farmem
ty(pe): 
sh(ort description): Normalize a far memory pointer
lo(ng description): Normalizes a far memory pointer. A normalized pointer
	has an offset smaller than 16.
pr(erequistes): 
va(lue): none: Micro-C
	\item normalize far pointer: Pacific HiTech C
re(lated to): _fcompare
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C and Pacific HiTech C

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

void _fnormalize_(unsigned *seg, unsigned *ofs)
{	*seg += *ofs >> 4;
	*ofs &= 0xf;
}

#else
#include <portable.h>
#include <dos.h>

void far *_fnormalize(void far *poi)
{	return	MK_FP(FP_SEG(poi) + (FP_OFF(poi) >> 4), FP_OFF(poi) & 0xF);
}

#endif
