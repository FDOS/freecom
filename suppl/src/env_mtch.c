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

ob(ject): env_matchVar
su(bsystem): env
ty(pe): 
sh(ort description): Retreive the name of a variable
lo(ng description): Retrieves the case-sensitive name of a variable.\par
	If no case-sensitive match is located within the environment, the
	first case-insensitive match is found and overwrites the passed
	in buffer of the name of the variable.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{1}: parameter out of range
	\item \tok{2}: no match found, \para{var][]} unchanged
	\item \tok{3}: no environment at all, \para{var][]} unchanged
	\nolist
	If \tok{(\para{return_value} & 7) == 0}, the return value is any combination
	of the following numbers:
	\item \tok{4}: more than one case-sensitive match found
	\item \tok{8}: more than one case-insensitive match found
	\item \tok{16}: at least one case-sensitive match found
	\item \tok{32}: at least one case-insensitive match found
	\nolist
	If \tok{(\para{return_value} & (4 | 16)) == 0}, the \tok{var[]} array
	is unchanged. Otherwise its contents is updated with the name of the first
	case-insensitively matched variable.
fi(le): env_mtch.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "fmemory.h"

#include "suppl.h"
#include "dynstr.h"
#include "environ.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_matchVar(word segm, char * const var)
{	int retVal, varLen;
	word firstOfs, ofs;

	DBG_ENTER("env_matchVar", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )

	chkMem
	
	if(!var) DBG_RETURN_I( 1)			/* no variable to search for */

	unless_segm(segm)
		DBG_RETURN_I( 3)				/* no environment at all */

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	retVal = ofs = 0;
	varLen = strlen(var);		/* offset of '=' sign */
	while(peekb(segm, ofs)) {
		if(peekb(segm, ofs + varLen) == '=') {	/* length is OK */
			if(_fmemcmp(MK_FP(segm, ofs), TO_FP(var), varLen) == 0) {
				/* case-sensitive match */
				retVal |= (retVal & 16)? 4: 16;
			}
			else if(_fMemiCmp(MK_FP(segm, ofs), TO_FP(var), varLen) == 0) {
				/* case-insensitive match */
				if(!retVal) firstOfs = ofs;
				retVal |= (retVal & 32)? 8: 32;
			}
		}
		ofs += env_varlen(segm, ofs);
	}

	if(!retVal)								/* no match found */
		DBG_RETURN_I( 2)

	if(!(retVal & (16 | 4)))		/* no case-sensitive match -> update var */
		_fmemcpy(TO_FP(var), MK_FP(segm, firstOfs), varLen);

	chkMem
	
	DBG_RETURN_BI( retVal & (32 | 16 | 8 | 4))
}
