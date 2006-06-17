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

ob(ject): dupvar
su(bsystem): env
ty(pe): H
sh(ort description): Duplicate a variable into heap
lo(ng description): Retreive the value of a variable of the default
	environment and duplicate its value into the heap.
pr(erequistes): 
re(lated to): env_dupvar cpyenv
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{NULL}: out of memory (ENOSPC), variable not found (ENOENT), no
	environment at all (ENXIO)
	\item else: pointer to allocated buffer
fi(le): env_dvar.c

ob(ject): env_dupvar
su(bsystem): env
ty(pe): H
sh(ort description): Duplicate a variable into heap
lo(ng description): Retreive the value of a variable of the specified
	environment and duplicate its value into the heap.
pr(erequistes): 
re(lated to): env_dupvar cpyenv
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{NULL}: out of memory or variable not found
	\item else: pointer to allocated buffer
fi(le): env_dvar.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "eno.loc"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dupvar(const char * const var)
{	return env_dupvar(0, var);	}
char *env_dupvar(word segm, const char * const var)
{	int ofs;						/* offset of the variable */
	char *h;

	DBG_ENTER("env_dupvar", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )

	chkMem
	unless_segm(segm) {
	 	eno_set(ENXIO);
 		DBG_RETURN_S( 0)
	}

	if(var == 0						/* nothing to do */
	 || (ofs = env_findVar(segm, var)) == -1) {	/* variable not found */
	 	eno_set(ENOENT);
		DBG_RETURN_S( 0)
	}

	if(0 == (h = _fdupstr(MK_FP(segm, ofs + strlen(var) + 1))))
		eno_set(ENOMEM);
	DBG_RETURN_S(h)
}
