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

ob(ject): Estrchg_
su(bsystem): error
ty(pe): 0H_
sh(ort description): Replace a pointer to a chunk dynamic memory
lo(ng description): Deallocates the current dynamic string of \tok{*var},
	duplicates \para{str} into the heap and stores the result into
	\tok{*var}.\par
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): \tok{var != NULL} and \tok{*var} must be either \tok{NULL} or a
	dynamically allocated chunk of memory or a dynamic string
va(lue): pointer to duplicated \para{str}
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
 *	Replace the contents of a string variable with another string
 */
char *Estrchg_(char ** const var, const char * const str)
{ 	DBG_ENTER("Estrchg_", Suppl_error)

	assert(var);

	DBG_ARGUMENTS( ("*var=%p, str=\"%s\"", *var, str) )

	chkHeap
	free(*var);
	chkHeap
	DBG_RETURN_BS( *var = Estrdup(str))
}
