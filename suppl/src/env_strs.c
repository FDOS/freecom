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

ob(ject): env_strings
su(bsystem): env
ty(pe): UM
sy(nopsis): 
sh(ort description): Return the number of environment strings
he(ader files): 
lo(ng description): Returns the number of strings appended to the
	environment area.
pr(erequistes): 
va(lue): 0: no environment (ENXIO) or no string (EZERO)
	\item else: number of environment strings
re(lated to): env_string env_strcounter
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

#include "environ.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#undef env_strings
int env_strings(word segm)
{	return env_strcounter(segm, 0);
}
