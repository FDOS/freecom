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

ob(ject): StrTrim
su(bsystem): dynstr
ty(pe): H
sh(ort description): Reallocate a dynstring
lo(ng description): Reallocates a dynamic string to its smallest possible
	buffer.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): The passed in string is \em{not} expected as a variable, thus,
	the result is not stored there, but returned as return value only.
bu(gs): 
fi(le): dstrtrim.c

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

char *StrTrim(char * const s)
{	char *p;
	DBG_ENTER("StrTrim", Suppl_dynstr)

	if(!s)
		DBG_RETURN_S( 0)

	chkHeap
	if((p = eno_realloc(s, strlen(s) + 1)) == 0) {
		DBG_STRING("realloc() should never have failed!!")
		chkHeap
		DBG_RETURN_S( s)
	}

	chkHeap
	DBG_RETURN_S( p)
}
