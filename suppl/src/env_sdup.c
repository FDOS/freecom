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

ob(ject): env_strdup
su(bsystem): env
ty(pe): H
sy(nopsis): 
sh(ort description): Duplicate an environment string into the heap
he(ader files): 
lo(ng description): Duplicates an environment string into the
	heap. The string must be deallocated by the user.
pr(erequistes): 
va(lue): \tok{NULL}: on failure; errno is assigned
		\list \tok{ENXIO}: no environment found
			\tok{ENOENT}: no such string (string number too high)
			\tok{ENOMEM}: out of memory
		\endlist
	\item else: pointer to allocated string
re(lated to): env_string
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#endif
#include "environ.loc"
#include "fmemory.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *env_strdup(word segm, const int index)
{	char *buf;
	word ofs;
	unsigned length;

	DBG_ENTER("env_strdup", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, idx=%d", segm, index) )

	chkMem

	unless_segm(segm) {
		eno_set(ENXIO);
		DBG_RETURN_S( 0)						/* no environment at all */
	}

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if((ofs = env_string(segm, index)) == 0 ||
	    ofs == env_firstFree(segm)) {
		eno_set(ENOENT);
		DBG_RETURN_S( 0)						/* string number too high */
	}

	chkHeap
	if((buf = eno_malloc(length = env_varlen(segm, ofs))) != 0)
		_fmemcpy(TO_FP(buf), MK_FP(segm, ofs), length);

	chkHeap
	DBG_RETURN_S( buf)
}
