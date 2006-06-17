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

ob(ject): env_findVar
su(bsystem): env
ty(pe): 
sh(ort description): Find a variable
lo(ng description): Searches case-sensitively for a variable in the given environment
pr(erequistes): segm != 0
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{(word)-1}: failure; not found (ENOENT), \tok{segm == 0 || val == 0} (EINVAL)
	\item else: offset of first case-sensitive match
fi(le): env_find.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.h"
#include "fmemory.h"

#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

word env_findVar(const word segm, const char * const var)
{	word offset;
	int len;

	DBG_ENTER("env_findVar", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )

	chkMem
	
	/* for security reason, check if environment is present at all */
	if(!segm || !var) {
		eno_set(EINVAL);
		DBG_RETURN_U( -1)			/* No env --> no var can be found */
	}
							/* No var --> nothing to search for */
	DBG_ARGUMENTS( ("effective env=%u", segm) )

	offset = 0;			/* env starts always at location 0 */
	len = strlen(var);

	while(peekb(segm, offset)) {			/* there is another variable */
		if(peekb(segm, offset + len) == '='
		 && _fmemcmp(MK_FP(segm, offset), TO_FP(var), len) == 0)
			DBG_RETURN_U( offset)		/* we found it */

		/* skip to the next variable */
		offset += env_varlen(segm, offset);
	}

	/* all variables were tested, nothing found */
	eno_set(ENOENT);
	DBG_RETURN_U( -1)
}
