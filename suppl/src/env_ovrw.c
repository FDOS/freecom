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

ob(ject): env_ovrVarOffset
su(bsystem): env
ty(pe): L
sh(ort description): Overwrite a variable
lo(ng description): Overwrites the variable at a given offset in the
	environment.\par
	If \tok{\para{value} == NULL}, the variable is removed.\newline
	If \tok{\para{var} == NULL}, the name of the variable is kept
	unchanged.
pr(erequistes): The offset must be point into the variable area.
re(lated to): env_insVarOffset
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \endlist integer SUPPL error code\newline
	\item \tok{ESUPPL_OK}: success
	\item\tok{ESUPPL_INVENV}: corrupted environment
	\item\tok{ESUPPL_NOMEM}: environment too small
	\item\tok{ESUPPL_NOENV}: no environment at all
fi(le): env_ovrw.c

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

int env_ovrVarOffset(word segm, word ofs, const char *var
 , const char * const value)
{	int ch;
	int lenO, lenON, lenN, lenV, lenG;

	DBG_ENTER("env_ovrVarOffset", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, ofs=%u, var=\"%s\", value=\"%s\"", segm, ofs, var, value) )

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if(!value) {		/* delete variable */
		env_subVarOffset(segm, ofs);
		chkMem
	
		DBG_RETURN_I( ESUPPL_OK)
	}

	lenON = 0;		/* keep the length of the var name for later when
						the total length of the variable is checked to
						NOT exceed 32KB */

	if(!var) {		/* keep the current variable name */
		/* skip until the '=' sign is found */
		while((ch = peekb(segm, ofs)) != '=') {
			++ofs;
			++lenON;
			if(!ch)			/* env corrupt */
				DBG_RETURN_I( ESUPPL_INVENV)
		}
		var = "";		/* the name is preserved */
	}

	/* Moved assignments before if():
		Aggressive compiler optimizations may cause parts of this
		statement to not be calculated. -- 2000/0705 Ron Cemer */
	lenO = env_varlen(segm, ofs);
	lenG = (lenN = strlen(var)) + (lenV = strlen(value)) + 2;
	if(lenG + lenON < 0				/* single var may not exceed 32KB */
	 || (lenO < lenG				/* will consume more space */
	  && env_freeCount(segm) <= lenG - lenO))	/* not enough memory */
	 	DBG_RETURN_I( ESUPPL_NOMEM)

	/* adjust the following variable(s) and the string area */
	if(lenG != lenO)
		_fmemmove(MK_FP(segm, ofs + lenG), MK_FP(segm, ofs + lenO)
		 , env_firstFree(segm) - (ofs + lenO));

	chkMem
	if(lenN)
		_fmemcpy(MK_FP(segm, ofs), TO_FP(var), lenN);
	chkMem
	pokeb(segm, ofs + lenN, '=');
	chkMem
	_fmemcpy(MK_FP(segm, ofs + lenN + 1), TO_FP(value), lenV + 1);
	chkMem

	DBG_RETURN_I( ESUPPL_OK)
}
