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

ob(ject): mcb_primaryShell
su(bsystem): mcb
ty(pe): 
sh(ort description): Return the MCB address of the master shell
lo(ng description): Returns the MCB address of the master shell,
	which is the first launched shell when booting the DOS kernel.
pr(erequistes): 
va(lue): 0: no primary shell could be identified
	\item else: MCB of primary shell
re(lated to): 
se(condary subsystems): 
bu(gs): 
wa:+Because DOS shells corrupt the process tree, the master shell can
	not be identified for sure the "easy" way. This function tries to
	locate the master shell by some assumptions known to be valid
	often. In the future some checks might be added when 100% working
	algorithms are known for specific environment, e.g. for specific
	shells.\par
	The algorithm used to identify the master shell performs three
	individual tests:
	\enum Use the process INT-2E points to;
		\enum Use the first shell in the MCB chain;
		\enum Use the last shell that has its environment behind its PSP.
		\endlist
	With all the memory saving programs moving their data freely through
	the memory, all three may or may not return the correct environment.\par
	Therefore, all three checks are made and the result will be assumed
	as true, which comes twice.
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#ifndef NDEBUG
#include <stdio.h>
#endif
#include <portable.h>
#include "environ.h"
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static int firstShell(void *arg, word mcb)
/* mcb_walk function for "1st shell" */
{	if(isShell(mcb)			/* shell found */
	 && mcb_env(mcb)) {	/* environment found */
	 	*(unsigned *)arg = mcb;
	 	return 1;		/* don't cycle any further */
	}
	return 0;			/* cycle further */
}


static lastShell(void *arg, word mcb)
/* mcb_walk function for "last shell" */
{	if(isShell(mcb)				/* a shell found */
	 && mcb_env(mcb) > mcb)	/* its env block is behind shell */
	 	*(unsigned *)arg = mcb;
	return 0;					/* because last shell is searched -> cycle */
}

word mcb_primaryShell(void)
{	word shell2E, shellFirst, shellLast;	/* found shells via 1-3 */
#ifndef _MICROC_
	isr vec2E;
#endif

	DBG_ENTER("mcb_primaryShell", Suppl_mcb)

#ifdef _MICROC_
	get_vector(0x2E, &shell2E, &shellFirst);
#else
	get_isr(0x2E, vec2E);
	shell2E = FP_SEG(vec2E);
	shellFirst = FP_OFF(vec2E);
#endif
	if(!shell2E || shell2E == 0xffff		/* no valid segment */
	 || (unsigned)peekb(shell2E, shellFirst) == 0xCF) /* iret */
		shell2E = 0;		/* there is no INT-2E handler! */
	else {					/* there is a 2E handler installed */
		/* move to the handler's shell */
		shell2E = mcb_shell(SEG2MCB(shell2E)); /* OK, shell found */
	}

	shellFirst = shellLast = 0;
	mcb_walk(0, aF(firstShell), &shellFirst);
	mcb_walk(0, aF(lastShell), &shellLast);

	if(shell2E == shellFirst && shell2E == shellLast)
		DBG_RETURN_I( shell2E)

	if(shellLast == shellFirst)
		DBG_RETURN_I( shellLast)

	DBG_RETURN_I( 0)				/* nothing found */
}
