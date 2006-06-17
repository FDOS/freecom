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

ob(ject): env_string
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Return the address of an environment string
he(ader files): 
lo(ng description): Returns the offset of a string into the specified
	environment.\par
	If \tok{index == env_strings()}, the first unused byte behind the last
	string is returned.
pr(erequistes): 
va(lue): 0: on failure; index too large (ENOENT) or no environment (ENXIO)
	\item else: offset of string
re(lated to): env_stringcounter
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers):

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include "environ.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

word env_string(word segm, int index)
{	word ofs;

	DBG_ENTER("env_string", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, idx=%d", segm, index) )

	chkMem
	unless_segm(segm) {
		eno_set(ENXIO);
		DBG_RETURN_I( 0)
	}

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	/* the string number "index" maybe equal to the number of
		strings, in which case the proper position of a new string
		is to be returned. */
	if(peekw(segm, ofs = 1 + env_endOfVars(segm)) < index) {
		eno_set(ENOENT);
		DBG_RETURN_I( 0)		/* string number too high */
	}

	ofs += 2;				/* 1st string */
	while(index--)		/* skip current string */
		ofs += env_varlen(segm, ofs);

	DBG_RETURN_I( ofs)
}
