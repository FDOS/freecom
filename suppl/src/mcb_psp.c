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

ob(ject): isPSP
su(bsystem): mcb
ty(pe): 
sh(ort description): Test if a MCB is a PSP
lo(ng description): Tests if the specified MCB hosts a valid PSP.\par
	Strictly applying DOS ways, there is no way to tell if a memory
	block is a PSP or not. A process could decide to \em{not} use
	particular functions of DOS and, therefore, could mis-use the
	fields of a PSP reserved for these functions. As a result, a valid
	PSP would look terrible wrong.\par
	Also, for efficiency reasons (speed namely) this function performs no
	test, if the passed in MCB is valid itself; as it is assumed that
	this function will be called when the MCB has been aquired from
	a \tok{mcb_walk()} loop or similiar function.\par
	This function decides from two prerequisites, if the MCB is a PSP:
	\enum The MCB must be owned by itself, and
	\enum the memory block starts with a specific magic number.
	\endlist
	The first prerequisite applies to all PSPs, because the PSP is immediately
	prepended to the image of the associated process, thus, the field
	"owner process" of the MCB points to itself.\newline
	The second prerequisite shall identify user-installed permanent
	memory blocks, where the programmer decided to use the above option
	to trick DOS to not remove the memory block, if the process terminates;
	thus, to leave the memory block in memory permanently.
pr(erequistes): 
va(lue): 0: is not a PSP
	\item else: is a PSP
re(lated to): isMCB isShell isEnv
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

int isPSP(const word mcb)
{	DBG_ENTER("isPSP", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )

			/* MCB is owned by itself */
	DBG_RETURN_BI( peekw(mcb, MCB_OFF_OWNER) == MCB2SEG(mcb)
			/* and starts with the magic number */
	 && peekw(mcb, SEG_OFFSET) == 0x20cd)
}
