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

ob(ject): env_fetch
su(bsystem): env
ty(pe): H
sh(ort description): Retreive the value of a variable
lo(ng description): Retreives the value of a variable. If the environment
	contains a case-sensitively matched variable, its value is returned,
	otherwise the first case-insensitive match.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{NULL}: failure; out of memory (ENOMEM), no variable found (ENOENT),
		no environment at all (ENXIO)
	\item else: pointer to internal buffer holding the value, its
	contents is overwritten the next time \tok{env_fetch()} is called.
fi(le): env_ftch.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "fmemory.h"

#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *env_fetch(word segm, const char * const var)
{	word exact, icase;
	static char *last = 0;

	DBG_ENTER("env_fetch", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )
	
	chkHeap
	unless_segm(segm) {
		eno_set(ENXIO);
		DBG_RETURN_S( 0)			/* no segment */
	}
	if(env_findAnyVar(segm, var, &exact, &icase)) {
		eno_set(ENOENT);
		DBG_RETURN_S( 0)			/* not found */
	}

	free(last);
	chkHeap
	DBG_RETURN_BS( last = eno_fdupstr(MK_FP(segm
	 , strlen(var) + 1 + (exact == (word)-1? icase: exact))))
}
