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

ob(ject): dpeekb
su(bsystem): farmem
ty(pe): 
sh(ort description): Extract a byte from a far memory table
he(ader files): 
lo(ng description): Extracts an unsigned byte from a far memory table.
	The function is not implemented as a macro, because that would
	evaluate at least one of the arguments twice.
pr(erequistes): tbl != NULL && abs(idx) < INT_MAX
va(lue): unsigned byte at the location
re(lated to): dpeekw peekw dpokeb
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dpeekb.c
co: Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dpeekb(fartbl tbl, int idx)
{	assert(tbl);
	return peekb(tbl->hi, tbl->lo + idx);
}

#endif
