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

ob(ject): longshrn
su(bsystem): longmath
ty(pe): A
sh(ort description): Shift a 32bit \para{n} bits to the right
lo(ng description): 
pr(erequistes): 
va(lue): none; \tok{num := num >> n} -or- \tok{num := num / 2^n}
re(lated to): longdivu longshl longshr longshln
se(condary subsystems): portable
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): A function for non-32bit-aware compilers, a macro otherwise.

*/

#include "initsupl.loc"

#ifdef _MICROC_

#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void longshrn(dword *num, unsigned cnt)
{	assert(num);
	if(cnt >= sizeof(dword) * 8) {
		longclr(num);
		return;
	}
	if(cnt >= sizeof(word) * 8) {
		num->lo = num->hi >> (cnt - sizeof(word) * 8);
		num->hi = 0;
		return;
	}
	while(cnt--)
		longshr(num)
}
#endif
