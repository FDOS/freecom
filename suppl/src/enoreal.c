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

ob(ject): eno_realloc
su(bsystem): error
ty(pe): HL
sy(nopsis): 
sh(ort description): as realloc(), but assigns errno
he(ader files): 
lo(ng description): as realloc(), but assigns errno = ENOMEM on failure
pr(erequistes): 
va(lue): NULL: on out-of-memory
 \item else: dynamically allocated block of memory
re(lated to): eno_strdup, Strdup, malloc
se(condary subsystems): mem
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <portable.h>

#include "eno.loc"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void *eno_realloc(void * const poi, const size_t len)
{	void *h;

	DBG_ENTER("eno_realloc", Suppl_error)
	DBG_ARGUMENTS( ("poi=%p, size=%u", poi, len) )

	if(0 == (h = realloc(poi, len)))
		eno_set( ENOMEM);
	DBG_RETURN_P(h)
}
