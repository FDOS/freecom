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

ob(ject): env_master
su(bsystem): env
ty(pe): 
lo: Returns the environment segment of the master shell,
	the first launched shell of the system
sh: Return the environment of the master shell
pr(erequistes): 
re(lated to): mcb_primaryShell
se(condary subsystems): 
in(itialized by): 
wa(rning): There is no known 100% failsafe way to get the master shell in
	DOS in all circumstances.
bu(gs): 
va(lue): \tok{0}: on failure
	\item else: segment of the mast environment
fi(le): env_mstr.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

unsigned env_master(void)
{	unsigned shell;

	DBG_ENTER("env_master", Suppl_env)

	chkMem

	if((shell = mcb_primaryShell()) != 0)
		DBG_RETURN_BU( mcb_env(shell))

	DBG_RETURN_U( 0)
}
