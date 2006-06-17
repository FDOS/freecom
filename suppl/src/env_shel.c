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

ob(ject): env_shell
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Return the environment of the effective shell
he(ader files): 
lo(ng description): Returns the default environment of the shell,
	that had spawned the current process.
pr(erequistes): 
va(lue): 0: The shell has no default environment or no parent shell
	\item else: segment address of the environment
re(lated to): mcb_shell
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
#else
#include <external.h>
#endif
#include <portable.h>
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

unsigned env_shell(void)
{	unsigned mcb;

	DBG_ENTER("env_shell", Suppl_env)

	DBG_RETURN_BU( (mcb = mcb_shell(SEG2MCB(_psp))) != 0
	 ? mcb_env(mcb)		/* MCB found -> return its environment segment */
	 : 0)
}
