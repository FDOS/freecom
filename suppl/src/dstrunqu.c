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

ob(ject): StrUnquote
su(bsystem): dynstr
ty(pe): H
sy(nopsis): 
sh(ort description): Remove any quote from the string
he(ader files): 
lo(ng description): Removes any quote from the string.\newline
	If quotes remain within the result, they were quoted within the
	string and therefore belong into the string.\newline
	Missing right quotes or single quotes immediately preceeding
	the '\0' byte are ignored silently.
pr(erequistes): quotes != NULL
va(lue): NULL: on memory allocation error (ENOMEM)
	\item else: pointer to dynamically allocated buffer
re(lated to): StrQuote StrUnquoteToken
se(condary subsystems): 
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
#endif
#include <portable.h>

#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrUnquote(const char * const s, const STR_QUOTES * const quotes)
{	char *result;
	const char *p;			/* temp pointer into s */

	DBG_ENTER("StrUnquote", Suppl_dynstr)

	assert(quotes);

	DBG_ARGUMENTS( ("str=\"%s\"", s) )

	chkHeap
	result = eno_strdup("");	/* if string is empty --> return either
								error, if strdup() returned NULL, or
								an empty string */
	p = s;
	chkHeap
	while(StrUnquoteTokenAppend(&p, quotes, &result));

	chkHeap
	DBG_RETURN_S(result)
}
