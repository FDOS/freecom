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

ob(ject): mcb_walk
su(bsystem): mcb
ty(pe): 
sh(ort description): Enumerate all remaining MCBs of the chain
lo(ng description): Enumerates all remaining MCBs of the chain. The callback
	function pointed to by \para{fct} is invoked for each found MCB.
	The function must comply to the \tok{MCB_WALKFUNC} type and gets
	passed in the unchanged \para{arg} parameter and the address of
	the found MCB. The callback function returns if the
	enumeration process shall proceed (return value equal to \tok{0} (zero))
	or terminate (returns anything unequal to zero).\par
	If \tok{mcb == 0}, the enumeration begins with the first MCB of the
	system.\par
	During the enumeration process UMBs, if any, are linked into the
	chain. They are unlinked upon return of \tok{mcb_walk()}.
pr(erequistes): \para{fct} != NULL
va(lue): -1: MCB chain corrupted (maybe returned by the callback
		function, too)
	\item 0: all MCB entries enumerated, the callback function always returned
		zero itself
	\item else: the value returned by the callback, unequal to zero
re(lated to): MCB_WALKFUNC mcb_allParents
se(condary subsystems): 
bu(gs): 
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
#include "mcb.h"
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int mcb_walk(word mcb, const MCB_WALKFUNC fct, void * const arg)
{	word ofs;
	FLAG UMBLink;
	FLAG8 MCBtype;
	USEREGS

	DBG_ENTER("mcb_walk", Suppl_mcb)
	DBG_ARGUMENTS( ("mcb=%u", mcb) )

	assert(fct);

	if(!mcb)		/* begin with the 1st MCB */
		mcb = mcb_first();

	DBG_ARGUMENTS( ("effective mcb=%u", mcb) )

	/* for walking, link in UMB list */
	UMBLink = 1;		/* Don't unlink UMBs */
	{
		_AX = 0x5802;	/* Get UMB Link state */
    /* /// Modified to use __emit__(), which doesn't require an assembler,
       if we're compiling with TurboC.  - Ron Cemer */
#if defined(_TC_EARLY_)
	        __emit__((unsigned char)0xf9);      /* stc */
#elif defined(__WATCOMC__) || defined(__GNUC__)
		reg.x.flags |= 1;
#else
		asm {
			stc
		}
#endif
		geninterrupt(0x21);
		if(!_CFLAG && !_AL) {	/* There are UMBs && not linked, yet */
			UMBLink = _AL;
			DBG_STRING("Link in UMBs")
			_BX = 1;	/* Link them */
			_AX = 0x5803;	/* Set UMB Link state */
			geninterrupt(0x21);
		}
	}

	while(ofs = -1, (MCBtype = peekb(mcb, MCB_OFF_TYPE)) == 'M' || MCBtype == 'Z') {
		/* MCB seems to be valid */

		if((ofs = (*fct)(arg, mcb)) != 0	/* call fct & break if necessary */
		 || MCBtype == 'Z') break;	/* end of chain */
		mcb += peekw(mcb, MCB_OFF_SIZE) + 1;	/* advance to next MCB */
	}

	if(UMBLink == 0) {	/* unlink UMBs */
		DBG_STRING("Unlink UMBs")
		_AX = 0x5803;
		_BX = 0;
		geninterrupt(0x21);
	}

	DBG_RETURN_I( ofs)
}
