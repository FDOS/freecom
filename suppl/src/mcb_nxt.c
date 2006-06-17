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

ob(ject): nxtMCB
su(bsystem): mcb
ty(pe): 
sh(ort description): Skip to next MCB in chain
lo(ng description): Skips to next MCB in the chain. To enrich the
	error detection mechanism the functions fails, if the passed in
	MCB \para{mcb} points to does not have the magic number 'M'.
pr(erequistes): mcb != 0
va(lue): 0: no further MCB in chain
	\item else: address of next MCB 
re(lated to): isMCB
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

word nxtMCB(word mcb)
{	DBG_ENTER("nxtMCB", Suppl_mcb)
	DBG_ARGUMENTS( ("MCB=%u", mcb) )
	assert(mcb);

	DBG_RETURN_BI( peekb(mcb, 0) == 'M'
		? MCB2SEG(mcb) + peekw(mcb, MCB_OFF_SIZE)
		: 0)
}
