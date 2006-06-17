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

ob(ject): isShell
su(bsystem): mcb
ty(pe): 
sh(ort description): Test if a MCB contains a shell
lo(ng description): Tests if the specified MCB is a valid PSP and
	also hosts a shell
pr(erequistes): 
va(lue): 0: is no shell
	\item else: is a shell
re(lated to): isMCB isPSP isEnv
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

int isShell(const word mcb)
{	DBG_ENTER("isShell", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )
	DBG_RETURN_BI( isPSP(mcb) && peekw(mcb, 0x16 + SEG_OFFSET) == MCB2SEG(mcb))
}
