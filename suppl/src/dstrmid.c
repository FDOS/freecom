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

ob(ject): StrMiddle
su(bsystem): dynstr
ty(pe): H
sh(ort description): Duplicate the n characters of a string into the heap
lo(ng description): Duplicates the \para{lenght} characters at position
	\para{pos} of the string \para{str} into the heap.\par
	The first character has position \tok{0} (zero).\newline
	If \para{pos} is greater than the length of the string, an empty string
	is created.\newline
	If less then \para{length} characters are left, the function
	acts like \tok{StrTail(pos)}, returning the remainder of the string.
pr(erequistes): 
re(lated to): 
va: NULL: \tok{s == NULL} (EINVAL) or \tok{malloc()} failed (ENOMEM)
	\item else: pointer to the new dynamic string
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrmid.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#endif
#include <portable.h>
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrMiddle(const char * const str, size_t pos, size_t length)
{
	DBG_ENTER("StrMiddle", Suppl_dynstr)

	if(!str) {
		eno_set(EINVAL);
		DBG_RETURN_S(0)
	}

	DBG_ARGUMENTS( ("str=\"%s\", pos=%u, len=%u", str, pos, length) )

	chkHeap
	DBG_RETURN_BS( strlen(str) <= pos? eno_strdup(""): StrLeft(str + pos, length))
}
