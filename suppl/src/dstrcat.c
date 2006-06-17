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

	char *StrCat_(char *dest[], char src[])

	Append the string src[] at dest.
	If src == NULL, this function performs no action.
	If *dest == NULL, this functions performs StrCpy_(dest, src)

	Return:
		NULL: malloc() failed; *dest is unchanged
		else: *dest

	Target compilers: Micro-C v3.13, v3.14; Borland C v2, v3.1, v4.52

ob(ject): StrCat_
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Append a string to a dynstr
lo(ng description): Appends a string to a dynamic string
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{*dst} may be changed
	\item else: pointer to new location, also in \tok{*dst}
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrcat.c

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

char *StrCat_(char ** const dest, const char * const src)
{	char *h;

	DBG_ENTER("StrCat_", Suppl_dynstr)
	assert(dest);
	DBG_ARGUMENTS( ("dst=\"%s\", src=\"%s\"", dest, src) )

	if(!src) DBG_RETURN_S( *dest)

	chkHeap
	if((h = eno_realloc(*dest, Strlen(*dest) + strlen(src) + 1)) == 0)
		DBG_RETURN_S( 0)

	chkHeap
	DBG_RETURN_BS( strcat(*dest = h, src))
}
