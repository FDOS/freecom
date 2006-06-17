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


ob(ject): isMCB
su(bsystem): mcb
ty(pe): 
sh(ort description): Test if a MCB is valid
lo(ng description): Tests if a given MCB is valid.
pr(erequistes): 
va(lue): 0: is no MCB\item else: is a MCB
re(lated to): isPSP isEnv mcb_walk
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <portable.h>
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static isMCBcomp(void *arg, word mcb)
{	return mcb == (unsigned)arg;
}

int isMCB(const word mcb)
{	DBG_ENTER("isMCB", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )
	DBG_RETURN_BI( mcb_walk(0, aF(isMCBcomp), (void*)mcb))
}
