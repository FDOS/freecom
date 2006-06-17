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

	char *StrLeft(char s[], unsigned length)

	Return the left length characters of s.

	If strlen(s) < length, the string is duplicated.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed buffer

	Target compilers: Micro-C, Borland C v2, v3.1, v4.52

ob(ject): StrLeft
su(bsystem): dynstr
ty(pe): H
sh(ort description): Duplicate the first n characters into the heap
lo(ng description): Duplicates the first \para{len} characters of the
	string \para{str} into the heap.
pr(erequistes): 
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{s == NULL} (EINVAL)
	\item else: pointer to dynamically allocated memory
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrleft.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif
#include <portable.h>
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrLeft(const char * const s, size_t length)
{	char *h;
	unsigned l;

	DBG_ENTER("StrLeft", Suppl_dynstr)

	if(!s) {
		eno_set(EINVAL);
		DBG_RETURN_S( 0)
	}

	DBG_ARGUMENTS( ("str=\"%s\", len=%u", s, length) )

	if((l = (unsigned)strlen(s)) < length)
		length = l;

	chkHeap
	if((h = eno_malloc(length + 1)) != 0) {
		memcpy(h, s, length);
		h[length] = NUL;
	}

	chkHeap
	DBG_RETURN_S( h)
}
