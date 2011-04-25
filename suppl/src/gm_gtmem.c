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

ob(ject): Emalloc
su(bsystem): error
ty(pe): 0H
sh(ort description): Allocate a chunk of memory
lo(ng description): Allocates a new chunk of memory from the heap.\par
	If \tok{size == 0}, the behaviour is undefined.\newline
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): 
va(lue): pointer to newly allocated memory
re(lated to): 
se(condary subsystems): dynstr
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif /*#	!defined(_MICROC_) */
#include <portable.h>
#include "msgs.h"

#include "dynstr.h"


#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif /*#	defined(RCS_Version) */

/*
 *	Allocate a chunk of memory
 *	On failure: Terminate with an error message
 */
void *Emalloc(size_t size)
{	byte *h;

	DBG_ENTER("Emalloc", Suppl_error)
	DBG_ARGUMENTS( ("len=%u", size) )

	chkHeap
	if((h = malloc(size)) == 0)
		Esuppl_noMem();

	chkHeap
	DBG_RETURN_P( h)
}
