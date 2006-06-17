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

ob(ject): mcb_owner
su(bsystem): mcb
ty(pe): 
sh(ort description): Return the owner of a MCB
lo(ng description): Returns the MCB of the PSP of the process that owns the
	specified MCB. Each MCB does have an owner.
pr(erequistes): 
va(lue): 0: MCB chain corrupted, MCB is unused or owned by the system
	\item else: MCB of owner process
re(lated to): mcb_walk
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

word mcb_owner(word mcb)
{	word mcb1;

	DBG_ENTER("mcb_owner", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )

	/* A PSP can be identified that the MCB has stored itself as the
		owner process. In some circumstances the "owner" field does
		not mention a process, but some other memory block allocated
		by a process. This loop resolves this situation and correctly
		returns a MCB of a PSP.

		Unused and system MCBs force this function to return with
		value "0" (zero) meaning "no owner found".
	*/
	do if((mcb1 = mcb) <= 0x40) {
		DBG_RETURN_I( 0)
	} while((mcb = peekw(mcb, MCB_OFF_OWNER)) != 0
	 && (mcb = SEG2MCB(mcb)) != mcb1);

	DBG_RETURN_I( mcb)
}
