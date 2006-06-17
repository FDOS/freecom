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

ob(ject): mcb_allParents
su(bsystem): mcb
ty(pe): 
sh(ort description): Enumerate all parents of a process
lo(ng description): Enumerates all parents of a process and launches
	the given function for each of them. \em{Note:} Because DOS shells
	patch their own "parent process ID", the last enumerated process is
	the shell the process was invoked by. This behaviour makes it
	unable to create full process trees known from Unix systems.\par
	If \tok{mcb == 0}, the enumeration begins with the current process;
	otherwise, the segment is probed via \tok{isPSP()} for validity.\newline
	The callback function \para{fct} must have the type \tok{MCB_WALKFUNC},
	which is a pointer to a function:\example{|}
	|int MCB_WALKFUNC(void *arg, unsigned mcb)
	The first parameter \para{arg} is the unchanged parameter \para{arg} passed
	into \tok{mcb_allParents} itself and \para{mcb} is the MCB ID of the
	enumerated process. If the callback function returns \tok{0} (zero),
	the enumeration process proceeds, otherwise it is terminated.
pr(erequistes): fct != NULL
va(lue): -1: on failure, e.g. corrupted MCB chain or invalid passed in
	\para{mcb}
	\item 0: callback function always returned \tok{0} (zero) itself and
		there is no parent for the currently enumerated process
	\item else: the value returned by the callback function, if it is
		non-zero
re(lated to): MCB_WALKFUNC mcb_walk
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

int mcb_allParents(word mcb, MCB_WALKFUNC fct, void *arg)
{	word mcb1;

	DBG_ENTER("mcb_allParents", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )

	assert(fct);

	if(mcb) {
		if(!isPSP(mcb)) {
			DBG_STRING("Invalid MCB")
			DBG_RETURN_I( -1)
		}
	}
	else mcb = _psp;

	DBG_ARGUMENTS( ("effective mcb=%u", mcb) )

	do {
		if((mcb1 = peekw(mcb, SEG_OFFSET + 0x16)) == mcb)
			DBG_RETURN_I( 0)		/* no parent process */
		if((mcb1 = (*fct)(arg, mcb = mcb1)) != 0)	/* function terminates */
			DBG_RETURN_I( mcb1)
	} while(mcb > 0x40);

	DBG_STRING("Corrupted MCB chain")
	DBG_RETURN_I( -1)
}
