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

ob(ject): mcb_shell
su(bsystem): mcb
ty(pe): 
sh(ort description): Return the MCB of the shell of a process
lo(ng description): Returns the MCB of the PSP of the shell that
	launched the process owning the specified MCB. If \para{mcb} already
	points to a shell, this shell is returned.
pr(erequistes): 
va(lue): 0: MCB chain corrupted, no hosting shell, \para{mcb} is
	unused or owned by the system
	\item else: MCB of PSP of the hosting shell
re(lated to): mcb_primaryShell mcb_allParents mcb_owner
se(condary subsystems): 
bu(gs): 
co(mpilers): 

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

word mcb_shell(word mcb)
{	word mcb1;

	DBG_ENTER("mcb_shell", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )

	if((mcb = mcb_owner(mcb)) != 0) {	/* go to mcb's owner PSP */
		DBG_ARGUMENTS( ("mcb of process = %u", mcb) )

		/* The "peekw()" reads the "Parent PSP" field of the PSP at
			MCB. A shell is identified as, in DOS, shells always
			store their own PSP in there, thus, interrupting the
			process tree.
		*/
		do if((mcb1 = mcb) <= 0x40)	{
			DBG_RETURN_I( 0)
		} while((mcb = peekw(mcb, SEG_OFFSET + 0x16)) != 0
		 && (mcb = SEG2MCB(mcb)) != mcb1);
	}

	DBG_RETURN_I( mcb)
}
