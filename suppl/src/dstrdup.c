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

	char *StrDup(char src[])

	Duplicate the string src.

	src may be NULL.

	Return:
		NULL: src == NULL || malloc() failed
		else: pointer to the malloc()'ed string

ob(ject): StrDup
su(bsystem): dynstr
ty(pe): H
sh(ort description): Duplicate a string into the heap
lo(ng description): Duplicates a string into the heap
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{src == NULL} (EINVAL)
	\item else: pointer of dynamically allocated memory
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrdup.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
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

char *StrDup(const char * const src)
{	DBG_ENTER("StrDup", Suppl_dynstr)
	if(src)
		DBG_RETURN_BS( eno_strdup(src))
	eno_set(EINVAL);
	DBG_RETURN_BS( 0)
}
