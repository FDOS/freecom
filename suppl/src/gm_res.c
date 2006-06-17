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

ob(ject): Eresize
su(bsystem): error
ty(pe): 0H_
sh(ort description): Resize a chunk of dynamic memory
lo(ng description): Resizes a chunk of dynamic memory.\par
	If \tok{*poi == NULL}, a new chunk is allocated.\newline
	If \tok{len == 0}, the chunk is deallocated
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): \tok{poi != NULL} and \tok{*poi} must be either \tok{NULL}
	or a chunk of dynamic memory or a dynamic string
va(lue): pointer to reallocated memory \tok{*poi}
re(lated to): 
se(condary subsystems): dynstr
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <assert.h>
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


void *Eresize_(void ** const poi, size_t len)
{	DBG_ENTER("Eresize", Suppl_error)

	assert(poi);

	DBG_ARGUMENTS( ("*poi=%p, new_len=%u", *poi, len) )

	chkHeap
	DBG_RETURN_BP( *poi = Erealloc(*poi, len))
}
