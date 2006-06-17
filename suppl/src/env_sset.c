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

ob(ject): env_setGlbSeg
su(bsystem): env
ty(pe): _
sy(nopsis): 
sh(ort description): Set the default environment
he(ader files): 
lo(ng description): Sets the default environment (current working
	environment) to the specified address. No actions are performed,
	other than to change the appropriate field in the process's PSP.
pr(erequistes): 
va(lue): none
re(lated to): env_glbSeg
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

 */

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>
#include <external.h>
#include "environ.h"

void env_setGlbSeg_(unsigned segm)
{	pokew(_psp, 0x2c, segm);
}
#endif
