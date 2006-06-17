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

ob(ject): env_findAnyVar
su(bsystem): env
ty(pe): 
sh(ort description): Find the offset a variable within the environment
lo(ng description): Find the offsets of both the first case-sensitively
	equal and case-insensitively equal variable. If one of them could not
	be located within the environment, the particular offset is set to
	\tok{(word)-1}, however, if no match could be found, both values
	remain unchanged.\par
	A case-sensitive match does \em{not} also match case-insensitively.\par
	Case-sensitive matches are performed via \tok{_fstrcmp()},
	case-insensitive ones via \tok{_fStriCmp()}.\par
	If \tok{\para{ecase} == \para{icase}}, the case-insensitive match is stored.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{ESUPPL_OK}: success, \tok{*\para{ecase}} and \tok{*\para{ecase}} changed
	\item \tok{ESUPPL_INVAL}: parameter out of range
	\item \tok{ESUPPL_NOENT}: no match,  \tok{*\para{ecase}} and \tok{*\para{ecase}} unchanged
	\item \tok{ESUPPL_NOENV}: no environment at all
fi(le): env_fany.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "fmemory.h"
#include "errcodes.h"
#include "suppl.h"
#include "dynstr.h"
#include "environ.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_findAnyVar(word segm, const char * const var
	, word *Xecase, word *Xicase)
{	word ecase, icase, ofs;
	int varLen;

	DBG_ENTER("env_findAnyVar", Suppl_env)

	chkMem
	
	if(!var || !Xicase || !Xecase)
		DBG_RETURN_I( ESUPPL_INVAL)

	DBG_ARGUMENTS( ("env=%u, var=\"%s\"", segm, var) )

	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)		/* no environment at all */

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	ofs = 0;
	varLen = strlen(var);		/* offset of '=' sign */
	ecase = icase = (word)-1;
	while(peekb(segm, ofs)) {
		if(peekb(segm, ofs + varLen) == '=') {	/* length is OK */
			if(ecase == (word)-1
			 && _fmemcmp(MK_FP(segm, ofs), TO_FP(var), varLen) == 0) {
				/* case-sensitive match */
				ecase = ofs;
				if(icase != (word)-1)
					break;
			}
			else if(icase == (word)-1
			 && _fMemiCmp(MK_FP(segm, ofs), TO_FP(var), varLen) == 0) {
				/* case-insensitive match */
				icase = ofs;
				if(ecase != (word)-1)
					break;
			}
		}
		ofs += env_varlen(segm, ofs);
	}

	if(ecase == (word)-1 && icase == (word)-1)		/* no match found */
		DBG_RETURN_I( ESUPPL_NOENT)

	*Xecase = ecase;
	*Xicase = icase;

	DBG_RETURN_VALUES( ("ecase=%u, icase=%u", ecase, icase) )

	DBG_RETURN_I( ESUPPL_OK)
}
