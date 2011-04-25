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

ob(ject): Estrdup
su(bsystem): error
ty(pe): 0H
sh(ort description): Duplicate a string into heap
lo(ng description): Duplicates a string into the heap.\par
	If \tok{s == NULL}, \tok{NULL} is returned.\newline
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): 
va(lue): NULL: if \tok{s == NULL}\item else: pointer to duplicated string
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
 *	Duplicate a string into the local heap.
 *	On failure: Terminate with an error message
 */
char *Estrdup(const char * const str)
{	char *h;

	DBG_ENTER("Estrdup", Suppl_error)

	if(!str) DBG_RETURN_S( 0)

	DBG_ARGUMENTS( ("str=\"%s\"", str) )

	chkHeap
	if((h = strdup(str)) == 0)
		Esuppl_noMem();

	chkHeap
	DBG_RETURN_S( h)
}
