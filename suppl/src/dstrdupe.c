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

	char *StrDupe(char *beg, char *end)

	Duplicate the range between beg through end (excluding *end);

	if beg == NULL, return NULL
	if end == NULL || end < beg, return dupstr(beg)

	Return:
		NULL: beg == NULL || out of memory
		else: duplicated string

ob(ject): StrDupe
su(bsystem): dynstr
ty(pe): H
sh(ort description): Duplicate a part of a string into the heap
lo(ng description): Duplicates a part of a string into the heap.\par
	The byte at \tok{*\para{end}} is excluded from the result.\newline
	If \tok{\para{end} == NULL || \para{begin} < \para{end}}, the
	whole string is duplicated,
pr(erequistes): 
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{beg == NULL} (EINVAL)
	\item else: pointer to dynamically allocated memory
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrdupe.c

*/

#include "initsupl.loc"

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif /*#	!defined(_MICROC_) */
#include <portable.h>
#include "msgs.h"
#include "eno.loc"

#include "dynstr.h"


#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif /*#	defined(RCS_Version) */

/*
 *	Duplicate a portion of a string
 */
char *StrDupe(const char * const beg, const char * const end)
{	char *p;
	size_t len;

	DBG_ENTER("StrDupe", Suppl_dynstr)
	DBG_ARGUMENTS( ("begin=%p, end=%p", beg, end) )

	if(!beg) {
		eno_set(EINVAL);
		DBG_RETURN_S( 0)
	}
	if(!end || end < beg) DBG_RETURN_BS( eno_strdup(beg))

	chkHeap
	if((p = eno_malloc(len = StrnLen(beg, (size_t)(end - beg)) + 1)) == 0)
		DBG_RETURN_S( 0)

	chkHeap
	p[--len] = '\0';			/* terminate the string */
	if(len)						/* make sure memcpy() doesn't fail */
		memcpy(p, beg, len);

	chkHeap
	DBG_RETURN_S( p)
}
