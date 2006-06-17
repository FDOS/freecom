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

ob(ject): Esetsize_
su(bsystem): error
ty(pe): 0H_
sh(ort description): Change the size of the chunk of dynamic memory
lo(ng description): Sets the size of a chunk of dynamic memory.\newline
	\em{Note:} Unlike \tok{Eresize_} the original contents is not
	preserved.\par
	If \tok{*poi == NULL}, a new chunk of memory is allocated.\newline
	If \tok{len == 0}, the memory is deallocated and this function
	returns \tok{NULL}.\newline
	If both \tok{*poi == NULL && len == 0}, the behaviour is undefined.
	\newline
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): \tok{poi != NULL} and \tok{*poi} must be either \tok{NULL}
	or a dynamically allocate memory or a dynamic string
va(lue): NULL: if \tok{len == 0}\item else: pointer to allocated memory \tok{*poi}
re(lated to): Eresize_
se(condary subsystems): dynstr
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <assert.h>
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


void *Esetsize_(void ** const poi, size_t len)
{	DBG_ENTER("Eresize_", Suppl_error)

	assert(poi);

	DBG_ARGUMENTS( ("*poi=%p, new_len=%u", *poi, len) )

	chkHeap
	free(*poi);
	chkHeap
	DBG_RETURN_BP( *poi = Emalloc(len))
}
