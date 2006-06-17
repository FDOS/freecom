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
/*  $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): env_check
su(bsystem): env
ty(pe): 
sh(ort description): Check the environment
lo(ng description): Checks the environment block. This function is
	save toward all types of problems, including segment wrap, wrong string
	table count.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{0}: environment OK
	\item \tok{1}: no environment at all
	\item \tok{2}: MCB corrupt; the env is addressed with an invalid segment,
		e.g. it does not exist or is too large (>= 64KB).
	\item \tok{3}: variable space corrupted
	\item \tok{4}: no string table (a warning rather than an error)
	\item \tok{5}: string table corrupted
fi(le): env_chk.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "mcb.h"
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_check(const word Xsegm)
{	unsigned offset, length, len, cnt;
	word segm;

	DBG_ENTER("env_check", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", Xsegm) )

	unless_Xsegm(segm, Xsegm)
		DBG_RETURN_I( 1)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	chkMem

	if((length = mcb_length(segm)) == 0	/* invalid MCB */
	 || !(length + 1))	/* length not allowed for an environment */
		DBG_RETURN_I( 2)

	offset = 0;				/* all environments start at offset 0 */
	/* check variable's space */
	while(peekb(segm, offset)) {
		len = env_varlen(segm, offset);
		if(addu(&offset, len) || offset >= length)
			DBG_RETURN_I( 3)		/* segment warp || segment overflow */
	}

	/* check string table */
	if(offset > 0xffff - 3 || (offset += 3) >= length)
		DBG_RETURN_I( 3)

	cnt = peekw(segm, offset - 2);
	if(cnt == 0)
		DBG_RETURN_I( 4)			/* no string table */

	while(cnt--) {
		len = env_varlen(segm, offset);
		if(addu(&offset, len) || offset > length)
			DBG_RETURN_I( 5)		/* segment warp || segment overflow */
	}

	DBG_RETURN_I( 0)
}
