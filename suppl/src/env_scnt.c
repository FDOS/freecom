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

ob(ject): env_stringcounter
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Modify and return the counter of environment strings
he(ader files): 
lo(ng description): Adds \para{diff} to the current number of environment
	strings and return the result.
pr(erequistes): 
va(lue): 0: No environment (ENXIO) or no string (EZERO)\item else: Number of strings
re(lated to): env_string
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include "environ.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_strcounter(word segm, int diff)
{	word cntOfs;

	DBG_ENTER("env_strcounter", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, diff=%d", segm, diff) )

	chkMem
	
	unless_segm(segm) {
		eno_set(ENXIO);
		DBG_RETURN_I( 0)
	}

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	diff += peekw(segm, cntOfs = env_endOfVars(segm) + 1);
	if(!diff) eno_set(EZERO);
	pokew(segm, cntOfs, diff);

	DBG_RETURN_U( diff)
}
