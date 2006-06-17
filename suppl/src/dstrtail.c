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

	char *StrTail(const char s[], unsigned pos)

	Return the substring beginning at position pos.

	If strlen(s) < pos, "" is returned.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed string

	Target compilers: Micro-C, Borland C v2, v3.1, v4.52

ob(ject): StrTail
su(bsystem): dynstr
ty(pe): H
sh(ort description): Return the end of a string
lo(ng description): Returns the end of a string beginning at the specified
	position.
pr(erequistes): 
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{s == NULL} (EINVAL)
	\item else: pointer to new location, also in \tok{*dst}
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrtail.c

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

char *StrTail(const char * const s, unsigned pos)
{	DBG_ENTER("StrTail", Suppl_dynstr)

	if(!s) {
		eno_set(EINVAL);
		DBG_RETURN_S( 0)
	}

	chkHeap
	DBG_RETURN_BS( eno_strdup(((unsigned)strlen(s) <= pos)
	 ? ""								/* behind its end */
	 : s + pos))
}
