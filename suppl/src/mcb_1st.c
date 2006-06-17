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

ob(ject): mcb_first
su(bsystem): mcb
ty(pe): 
sh(ort description): Return the first MCB
lo(ng description): Returns the first MCB within the chain
pr(erequistes): 
va(lue): The first MCB
re(lated to):
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

word mcb_first(void)
{	word ofs, mcb;
	USEREGS

	DBG_ENTER("mcb_first", Suppl_mcb)

	_AH = 0x52;			/* get list of list */
	geninterrupt(0x21);
	ofs = _BX;			/* mcb:ofs := pointer to list of lists */
	mcb = _ES;

	if(ofs < 2) --mcb, ofs += 16;	/* Avoid the segment warp */

	DBG_RETURN_U( peekw(mcb, ofs - 2) )	/* 1st MCB */
}
