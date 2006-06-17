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

ob(ject): env_nullStrings
su(bsystem): env
ty(pe): 
sh(ort description): Empty string area
lo(ng description): Removes all strings from the string area of the
	environment. Can be called if \tok{env_check()} returned "invalid
	string area".
pr(erequistes): 
re(lated to): env_strings
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \endlist integer SUPPL error code\newline
	\item \tok{ESUPPL_OK}: success
	\item \tok{ESUPPL_NOMEM}: no room to write counter
	\item \tok{ESUPPL_NOENV}: no environment at all
fi(le): env_nost.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "errcodes.h"
#include "environ.loc"
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_nullStrings(word segm)
{	word ofs;

	DBG_ENTER("env_nullStrings", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	ofs = env_endOfVars(segm) + 1;		/* offset of string counter word */
	if(mcb_length(segm) - 2 <= ofs)
		DBG_RETURN_I( ESUPPL_NOMEM)

	pokew(segm, ofs, 0);
	chkMem
	
	DBG_RETURN_I( ESUPPL_OK)
}
