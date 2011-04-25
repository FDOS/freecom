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

ob(ject): suppl_log_unlock
su(bsystem): debug
ty(pe): 
sy(nopsis): 
sh(ort description): "Unlock" the debug/logging package
he(ader files): 
lo(ng description): "Unlocks" the debug/logging package to prevent
	recursively calling of these packages
pr(erequistes): 
va(lue): 
re(lated to): suppl_log_lock
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): suppl_log_lock
su(bsystem): debug
ty(pe): 
sy(nopsis): 
sh(ort description): "Lock" the debug/logging package
he(ader files): 
lo(ng description): "Locks" the debug/logging package to prevent
	recursively calling of these packages
pr(erequistes): 
va(lue): 
re(lated to): suppl_log_unlock
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
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static int locked = -1;

int suppl_log_lock(void)
{	if(++locked) { /* recursion */
		--locked;
		return 0;
	}

	return 1;
}

void suppl_log_unlock(void)
{	--locked;
}
