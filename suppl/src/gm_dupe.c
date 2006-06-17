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


ob(ject): Estrdupe
su(bsystem): error
ty(pe): 0H
sh(ort description): Duplicate part of a string into heap
lo(ng description): Duplicates a part of a string into the heap. The
	first character is pointed to by \para{beg} and the first
	character \em{not} included into the string is pointed to
	by \para{end}.\par
	If \tok{beg == NULL}, \tok{NULL} is returned.\newline
	If \tok{end == NULL || end < beg}, \tok{Estrdup(beg)} is returned.\newline
	On failure, the program is terminated with the error message:
	"Out of memory".
pr(erequistes): 
va(lue): pointer to duplicated string
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
 *	Duplicate a portion of a string
 */
char *Estrdupe(const char * const beg, const char * const end)
{	char *p;

	DBG_ENTER("Estrdupe", Suppl_error)

	if(!beg) DBG_RETURN_S( 0)

	DBG_ARGUMENTS( ((end && end >= beg)? "beg=\"%s\", len=%u"
		: "beg=\"%s\" end invalid", beg, end - beg) )

	chkHeap
	if((p = StrDupe(beg, end)) == 0)
		Esuppl_noMem();

	chkHeap
	DBG_RETURN_S( p)
}
