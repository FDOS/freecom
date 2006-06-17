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

ob(ject): mcb_length
su(bsystem): mcb
ty(pe): 
sh(ort description): Return the length of a memory block as \tok{(int)}
lo(ng description): Returns the length of a memory block in bytes, \em{but}
	in the range 0..65534!\par
	The specified value must be the address to the \em{memory segment} rather
	than the address of the associated MCB!
pr(erequistes): 
va(lue): 0: \tok{segm == 0} or the block is really empty
	\item \tok{0xFFFFu}: the block is larger than 64KB
	\item else: the size of the block at address \para{segm}, in range
		0..65520 bytes
re(lated to): 
se(condary subsystems): 
bu(gs): 
co(mpilers): 
wa:+ Unlike most of the functions of this subsystem the specified
	address must point to the real memory segment rather than its MCB!

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

unsigned mcb_length(const word segm)
/* Return the length of the memory block starting at segm */
{	unsigned length;

	DBG_ENTER("mcb_length", Suppl_mcb)
	DBG_ARGUMENTS( ("segm=%u", segm) )

	if(segm == 0) {			/* security value */
		DBG_STRING("Invalid memory segment")
		DBG_RETURN_I( 0)
	}

	length = peekw(SEG2MCB(segm), MCB_OFF_SIZE);	/* len of block in paras */

	DBG_RETURN_BI( length >= 4096?	/* larger than 64KB */
		~0: length << 4)
}
