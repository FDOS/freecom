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

ob(ject): env_strput
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Replace an environment string
he(ader files): 
lo(ng description): 
	Replaces the \para{index}'th environment string (1st string is 0)
	by the ASCIZ string buf.\par
	If \tok{\para{buf} == NULL}, the string is deleted, all following strings
	are shifted up one position.\newline
	If \tok{\para{index} == env_strings(\para{segm})}, a new string is appended.\newline
	If \tok{\para{index} == env_strings(\para{segm}) && \para{buf} == NULL},
	NOP (ignored).\newline
pr(erequistes): 
va(lue): integer SUPPL error code
re(lated to): env_string env_strinsert
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers):

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <dos.h>
#endif
#include "errcodes.h"
#include "environ.loc"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_strput(word segm, const char * const buf, const int index)
{	word ofs, len, lenX;
	int cnt;

	DBG_ENTER("env_strput", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, idx=%d, buf=\"%s\"", segm, index, buf) )

	chkMem

	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)			/* no environment at all */

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if((ofs = env_string(segm, index)) == 0)
		DBG_RETURN_I( ESUPPL_NOENT)			/* string number too high */

	if(index == (cnt = env_strcounter(segm, 0))) {	/* append string */
			/* If buf == NULL, ignore */
		DBG_RETURN_BI( buf? env_strinsert(segm, buf, index): 0)
	}

/* at this place segm:ofs points to a valid string */

	len = buf? strlen(buf) + 1: 0;		/* length of string to put there */
	lenX = env_varlen(segm, ofs);		/* length of current string */
	if((len > lenX) && (env_freeCount(segm) <= len - lenX))
		DBG_RETURN_I( ESUPPL_NOMEM)	/* not enough memory */

	if(index < cnt - 1) {	/* There are strings following the one
									to be replaced */

		/* Move them to the place directly where the new string will end */
		_fmemmove(MK_FP(segm, ofs + len), MK_FP(segm, ofs + lenX)
		 , env_firstFree(segm) - (ofs + lenX));
		chkMem
	}

/* Now, there is enough room at segm:ofs to put string there */

	if(buf) {		/* change the string value */
		_fmemcpy(MK_FP(segm, ofs), TO_FP(buf), len);
	}
	else 		/* The string has been deleted */
		env_strcounter(segm, -1);
	chkHeap

	DBG_RETURN_I( ESUPPL_OK)
}
