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

	char *StrRepl_(char *dest[], const char src[])

	Free dest[0] and place src there

	Return: src

ob(ject): StrRepl_
su(bsystem): dynstr
ty(pe): H_
sh(ort description): Replace a dynstring with a new dynstring
lo(ng description): Replaces a dynamic string with another dynamic string.\par
	\em{Note}: The new string must be a dynamic string already, it is \em{not}
	created anew by this function.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va: \para{src}
fi(le): dstrrepl.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <portable.h>
#include <dynstr.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrRepl_(char ** const dest, char * const src)
{	DBG_ENTER("StrRepl_", Suppl_dynstr)

	assert(dest);
	chkHeap
	free(*dest);
	chkHeap
	DBG_RETURN_BS( dest[0] = src)
}
