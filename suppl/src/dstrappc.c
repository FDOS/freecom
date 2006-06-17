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

ob(ject): StrAppChr_
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Append a character to a dynstring
he(ader files): 
lo(ng description): Appends a single character to a dynamic string
pr(erequistes): \tok{\para{dst} != NULL}
re(lated to): 
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{*dst} is unchanged
	\item else: pointer to new location, also in \tok{*dst}
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrappc.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrAppChr_(char ** const dst, char chr)
{	static char appBuf[] = " ";

	DBG_ENTER("StrAppChr_", Suppl_dynstr)

	assert(dst);

	DBG_ARGUMENTS( ("dst=\"%s\", chr='%c' (0x02x)", dst, chr > 32? chr: ' ', chr) )

	chkHeap
	if((*appBuf = chr) != NUL)
		DBG_RETURN_BS( StrCat_(dst, appBuf))

	chkHeap
	DBG_RETURN_S( *dst)
}
