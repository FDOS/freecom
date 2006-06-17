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

	char *StrCpy_(char *dest[], char src[])

	Copy (duplicate) the string src[] into dest.
	If src == NULL, this function is equal to StrFree(dest).

	Return:
		NULL: malloc() failed
		else: *dest

ob(ject): StrCpy_
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Copy (duplicate) a string into a dynstring
lo(ng description): Copies a string into a dynamic string
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{*dst == NULL}
	\item else: pointer to new location, also in \tok{*dst}
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): \tok{*dst} is not deallocated!
bu(gs): 
fi(le): dstrcpy.c

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

char *StrCpy_(char ** const dest, const char * const src)
{
	DBG_ENTER("StrCpy_", Suppl_dynstr)
	assert(dest);

	if(src) {
		size_t len = strlen(src);
		char *p;

		chkHeap
		if(0 != (p = eno_realloc(*dest, ++len))) {
			chkHeap
			memcpy(*dest = p, src, len);
			DBG_RETURN_S(*dest)
		}
	}
	DBG_RETURN_BS( StrFree_(dest) )
}
