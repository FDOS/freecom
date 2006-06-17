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

ob(ject): FFput
su(bsystem): supplio
ty(pe): 
sh(ort description): Write a buffer into a stream
lo(ng description): Writes a buffer into a stream and return
	the success status.
pr(erequistes): buf != NULL && f != NULL
va(lue): \tok{0}: on success
	\item else: on failure
re(lated to): FFget Fput
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int FFput(const void * const buf, size_t len, FILE * const f)
{	DBG_ENTER("FFput", Suppl_supplio)
	assert(buf);
	assert(f);
	DBG_RETURN_BI( Fput(buf, len, f) != len)
}
