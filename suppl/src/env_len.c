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
/*  $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): env_varlen
su(bsystem): env
ty(pe): 
sh(ort description): Return the number of bytes used by the variable
lo(ng description): Returns the number of bytes used by the variable
	at the specified offset, including \tok{NUL} terminator byte.
pr(erequistes): \para{segm} != NULL
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): number of bytes used by the variable
fi(le): env_len.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

unsigned env_varlen(const word segm, const word offset)
/* Return the length of the string at segm:offset */
{	assert(segm != 0);
	return _fstrlen(MK_FP(segm, offset)) + 1;
}
