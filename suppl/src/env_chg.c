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

ob(ject): env_change
su(bsystem): env
ty(pe): 
sh(ort description): Insert/replace/delete a variable
lo(ng description): If \tok{\para{value} != NULL}, the variable is
	inserted or replaced, if it already exists within the environment.\newline
	If \tok{\para{value} == NULL}, the variable is removed from the
	environment.\newline
	Both \para{var} and \para{value} may point to an empty string.
pr(erequistes): var != NULL
re(lated to): chgenv env_add
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{0}: Cannot insert variable
	\item \tok{1}: variable replaced or deleted
	\item \tok{2}: variable not found (could not delete)
	\item \tok{3}: variable newly inserted
	\item \tok{4}: no environment at all
	\item \tok{-1}: \tok{\para{varname} == NULL}
fi(le): env_chg.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_change(word segm, const char * const var, const char * const value)
{	unsigned offset;

	DBG_ENTER("env_change", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\", value=\"%s\"", segm, var, value) )

	chkMem
	if(var == 0)
		DBG_RETURN_I( -1)

	unless_segm(segm)
		DBG_RETURN_I( 4)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	/* Check if the variable is already in */
	if((offset = env_findVar(segm, var)) != 0xffff)
		/* Overwrite/delete variable */
		DBG_RETURN_BI( !env_ovrVarOffset(segm, offset, 0, value))

	/* not in environment */
	if(!value)			/* nothing to delete */
		DBG_RETURN_I( 2)

	/* Try to insert the variable */
	DBG_RETURN_BI( env_appVar(segm, var, value)? 0: 3)
}
