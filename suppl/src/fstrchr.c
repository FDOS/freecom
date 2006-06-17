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

ob(ject): _fstrchr
su(bsystem): farmem
ty(pe): 
sy(nopsis): 
sh(ort description): Search a far string for a character
he(ader files): 
pr(erequistes): pointer must not be \tok{NULL}
va(lue): 0: value not found\item else: offset of first occurence of character
re(lated to): 
se(condary subsystems): portable
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): If \tok{offset == 0} and the character is located at the first
	position of the string, \tok{0} is returned, too.
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

unsigned _fstrchr(unsigned const seg, unsigned ofs, const unsigned value)
{	unsigned char v, i;

	v = value;
	do {
		if((i = peekb(seg, ofs)) == v)
			return ofs;
		if(!++ofs)
			break;
	} while(i);

	return 0;
}

#else
#ifdef _TC_EARLY_
#include <portable.h>
#include "fmemory.h"

char far *_fstrchr(const char far* const s, int ch)
{	const byte far *p;

	p = (const byte far*)s;
	do {
		if(*p == ch)
			return (char far*)p;
	} while(*p++);

	return 0;
}

#endif

#endif
