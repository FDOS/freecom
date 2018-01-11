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

ob(ject): _fstrlen
su(bsystem): farmem
ty(pe): 
sh(ort description): Return the length of a far string
pr(erequistes): the pointer must not be \tok{NULL}
va(lue): length of far string
re(lated to): strlen
se(condary subsystems): portable
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>

unsigned _fstrlen(unsigned const seg, unsigned const ofs)
{	unsigned p;

	for(p = ofs; peekb(seg, p); ++p);

	return p - ofs;
}

#else

#if defined(_TC_EARLY_) || defined(__GNUC__)
#include <assert.h>
#include <portable.h>
#include "fmemory.h"

unsigned _fstrlen(const char far * const s)
{	const byte far *p = (const byte far*)s;

    while (*p++);
    assert((p - (const byte far*)s) <= 0x10000l);
    return (unsigned)(p - (const byte far*)s) - 1;
}

#endif

#endif
