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

ob(ject): env_appVar
su(bsystem): env
ty(pe): L
sh(ort description): Append a variable to the end of the environment
lo(ng description): Inserts a variable before the end of the
	variables area of the environment
pr(erequistes):
re(lated to): env_change putenv env_insVarOffset
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
va(lue): \endlist integer SUPPL error code\newline
	\item \tok{ESUPPL_OK}: variable inserted or \tok{\para{value} == NULL}
	\item \tok{ESUPPL_NOMEM}: failed to insert variable
	\item \tok{ESUPPL_INVAL}: parameter out of range
	\item \tok{ESUPPL_NOENV}: no environment at all
fi(le): env_insv.c

ob(ject): env_insVarOffset
su(bsystem): env
ty(pe): L
sh(ort description): Insert a variable into the environment
lo(ng description): Inserts a variable before the specified
	offset of an environment
pr(erequistes):
re(lated to): env_change putenv env_appVar
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
va(lue): \tok{ESUPPL_OK}: variable inserted or \tok{\para{value} == NULL}
	\item \tok{ESUPPL_NOMEM}: failed to insert variable
	\item \tok{ESUPPL_INVAL}: parameter out of range
	\item \tok{ESUPPL_NOENV}: no environment at all
fi(le): env_insv.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "errcodes.h"
#include "fmemory.h"
#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_appVar(word segm, const char * const var, const char * const value)
{	DBG_ENTER("env_appVar", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\", value=\"%s\"", segm, var, value) )

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	DBG_RETURN_BI( env_insVarOffset(segm, env_endOfVars(segm)
		, var, value))
}

int env_insVarOffset(word segm, word offset, const char * const var
 , const char * const value)
{	int lenN, lenV, lenG;

	DBG_ENTER("env_insVarOffset", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, ofs=%u, var=\"%s\", value=\"%s\"", segm, offset, var, value) )

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if(!value)		/* ignore */
		DBG_RETURN_I( ESUPPL_OK)
	if(!var)
		DBG_RETURN_I( ESUPPL_INVAL)

	if(env_freeCount(segm)
	 < (lenG = (lenN = strlen(var)) + (lenV = strlen(value)) + 2)
	  || lenG <= 1)		/* longer than 32KB */
		DBG_RETURN_I( ESUPPL_NOMEM)	/* sorry, no space free in the env seg */

	/* 1) move the following variables and the string table out of the way */
	_fmemmove(MK_FP(segm, offset + lenG), MK_FP(segm, offset)
	 , env_firstFree(segm) - offset);

	/* 2) move the variable */
	chkMem
	_fmemcpy(MK_FP(segm, offset), TO_FP(var), lenN);
	chkMem
	_fmemcpy(MK_FP(segm, offset + lenN + 1), TO_FP(value), lenV + 1);
	chkMem
	pokeb(segm, offset + lenN, '=');

	chkMem
	
	DBG_RETURN_I( ESUPPL_OK)
}
