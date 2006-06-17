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

	char *StrRight(char s[], unsigned length)

	Return the right length characters of a string.

	If strlen(s) < length, the string is duplicated.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed string

ob(ject): StrRight
su(bsystem): dynstr
ty(pe): H
sh(ort description): Return the right n bytes of a string
lo(ng description): Returns the rightmost \para{length} bytes of a string as dynamic
	string. If the string has less then \para{length} characters, all of the
	string is returned.
pr(erequistes): 
re(lated to): 
va: NULL: if \tok{malloc()} fails (ENOMEM)
	\item else: pointer to dynamically allocated memory
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrrigh.c

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

char *StrRight(const char * const s, size_t length)
{	unsigned l;

	DBG_ENTER("StrRight", Suppl_dynstr)

	if(!s) return 0;

	chkHeap
	DBG_RETURN_BS(((l = (unsigned)strlen(s)) <= length)
	 ? eno_strdup(s)								/* complete copy */
	 : StrTail(s, l - length))
}
