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

ob(ject): StrChg
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Reallocate/resize a dynamic string
lo(ng description): Resizes a dynamic string preserving the original
	contents.\par
	On failure, the original string at \tok{*poi} is deallocated and
	\tok{NULL} is assigned to \tok{*poi} itself.\newline
	For \tok{*poi} and \para{size} apply the same rules as if
	passed into \tok{realloc()}.
pr(erequistes): \tok{*poi == NULL} or \tok{*poi} is a pointer to a
	dynamically allocated memory
va(lue): \tok{*poi}
re(lated to): realloc
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <assert.h>
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrChg_(char ** const poi, size_t size)
{	char *p;

	DBG_ENTER("StrChg_", Suppl_dynstr)

	assert(poi);

	DBG_ARGUMENTS( ("*poi=%p, size=%u", poi, size) )

	chkHeap
	if((p = eno_realloc(*poi, size)) == 0)
		DBG_RETURN_BP( StrRepl_(poi, 0))

	chkHeap
	DBG_RETURN_BP( *poi = p)
}
