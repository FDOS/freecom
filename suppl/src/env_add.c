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

ob(ject): env_addVar
su(bsystem): env
ty(pe): L
sh(ort description): Append a new variable to an environment
lo(ng description): Appends a new variable to the end of an environment,
	no check is performed, if the variable does already exists in the
	environment.
pr(erequistes): name != NULL && value != NULL && segm != 0
re(lated to): chgenv env_change putenv
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): 0: on failure
	\item else: on success
fi(le): env_add.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.h"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


int env_addVar(const unsigned segm, const char * const name
	, const char * const value)
{	unsigned endOff, firstFree;
	int lenN, lenV, lenG;

	DBG_ENTER("env_addVar", Suppl_env)

	assert(name);
	assert(value);
	assert(segm);

	DBG_ARGUMENTS( ("env=%u, name=\"%s\", value=\"%s\"", segm, name, value) )

	chkMem

	if(env_freeCount(segm)
	 < (lenG = (lenN = strlen(name)) + (lenV = strlen(value)) + 2)
	  || lenG <= 1)		/* longer than 32KB */
		DBG_RETURN_I( 0)		/* sorry, no space free in the env seg */

	/* 1) move the string table out of the way */
	firstFree = env_firstFree(segm);
	endOff = env_endOfVars(segm);

	_fmemmove(MK_FP(segm, endOff + 1 - 1 + lenG), MK_FP(segm, endOff + 1 - 1)
	 , firstFree - (endOff + 1) + 1);	/* move the var space terminator, too */

	chkMem
	
	/* 2) move the variable */
	_fmemcpy(MK_FP(segm, endOff), TO_FP(name), lenN);

	chkMem
	
	_fmemcpy(MK_FP(segm, endOff + 1 + lenN), TO_FP(value), lenV + 1);

	chkMem
	
	pokeb(segm, endOff + lenN, '=');

	chkMem
	
	DBG_RETURN_I( 1)
}
