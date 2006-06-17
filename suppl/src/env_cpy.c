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

ob(ject): cpyenv
su(bsystem): env
ty(pe): 
sh(ort description): Get the value of a variable
lo(ng description): Retreives the value of a variable of the default
	environment into a given buffer
pr(erequistes): 
re(lated to): env_get dupenv env_dup env_findVar env_fetch
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): integer SUPPL error code
fi(le): env_cpy.c

ob(ject): env_get
su(bsystem): env
ty(pe): 
sh(ort description): Get the value of a variable
lo(ng description): Retreives the value of a variable of the specified
	environment into a given buffer
pr(erequistes): 
re(lated to): cpyenv dupenv env_dup env_findVar env_fetch
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): integer SUPPL error code
fi(le): env_cpy.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "errcodes.h"
#include "environ.loc"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int cpyenv(const char * const var, char * const dst, const unsigned length)
{	return env_get(0, var, dst, length);	}
int env_get(word segm, const char * const var, char * const dst
	, const unsigned length)
{	int ofs;
	int cpyLen;

	DBG_ENTER("env_get", Suppl_env)

	chkMem

	if(var == 0 || dst == 0 || length == 0)
		DBG_RETURN_I( ESUPPL_INVAL)		/* nothing to do */

	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )

/* Locate the environment segment */
	unless_segm(segm)	
		DBG_RETURN_I( ESUPPL_NOENV)


	DBG_ARGUMENTS( ("effective env=%u", segm) )

/* Locate the variable */
	if((ofs = env_findVar(segm, var)) == -1)	/* not found */
		DBG_RETURN_I( ESUPPL_NOENT)

	ofs += strlen(var) + 1;	/* skip the name & the equal sign */

/* Check, if the buffer overflows */
	if((cpyLen = _fstrlen(MK_FP(segm, ofs)) + 1) > length)
		cpyLen = length;

/* Copy the contents including the NUL terminator */
	_fmemcpy(TO_FP(dst), MK_FP(segm, ofs), cpyLen);
	if(cpyLen == length && dst[length - 1]) {		/* buffer overflow */
		dst[length - 1] = '\0';
		chkHeap
		DBG_RETURN_I( ESUPPL_RANGE)
	}

	chkHeap
	DBG_RETURN_I( ESUPPL_OK)
}
