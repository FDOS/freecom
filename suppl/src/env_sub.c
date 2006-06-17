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


ob(ject): env_subVarOffset
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Remove an environment variable
he(ader files): 
lo(ng description): Removes the environment variable at a
	specified offset
pr(erequistes): \tok{\para{segm} != 0}
	\item \para{offset} must point into the variables area of the
	environment
va(lue): none
re(lated to): env_insVarOffset env_ovrVarOffset env_change
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers):

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.h"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void env_subVarOffset(word segm, const word offset)
{	register unsigned moveStart;

	DBG_ENTER("env_subVarOffset", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, ofs=%u", segm, offset) )

	chkMem
	assert(segm);
	assert(offset < env_endOfVars(segm));

	moveStart = offset + env_varlen(segm, offset);

	_fmemmove(MK_FP(segm, offset), MK_FP(segm, moveStart),
		env_firstFree(segm) - moveStart);
	chkMem

	DBG_EXIT
}
